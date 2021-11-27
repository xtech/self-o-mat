//
// blocking_reader.h - a class that provides basic support for
// blocking & time-outable single character reads from
// boost::asio::serial_port.
//
// use like this:
//
// 	blocking_reader reader(port, 500);
//
//	char c;
//
//	if (!reader.read_char(c))
//		return false;
//
// Kevin Godden, www.ridgesolutions.ie
//

#ifndef SELF_O_MAT_BLOCKING_READER_H
#define SELF_O_MAT_BLOCKING_READER_H

#include <boost/asio/serial_port.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <iostream>
#include <tools/verbose.h>

/* Use the following macro to allow builds for different boost library versions;
 * inspired by: https://stackoverflow.com/a/67773642/1627585
 */
#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) ((boost::asio::io_context&)(s).get_executor().context())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service())
#endif

#define BRDR_TAG "BLOCKING_READER"


using namespace selfomat::tools;

class blocking_reader
{
    boost::asio::serial_port& port;
    size_t timeout;
    char c;
    boost::asio::deadline_timer timer;
    bool read_error;

    // Called when an async read completes or has been cancelled
    void read_complete(const boost::system::error_code& error,
                       size_t bytes_transferred) {
        LOG_D(BRDR_TAG, "read complete: bytes_transferred=" + std::to_string(bytes_transferred));
        read_error = (error || bytes_transferred == 0);

        if(read_error) {
            LOG_E(BRDR_TAG, "boost error message: ", error.message());
        }

        // Read has finished, so cancel the
        // timer.
        timer.cancel();
    }

    // Called when the timer's deadline expires.
    void time_out(const boost::system::error_code& error) {
        // Was the timeout cancelled?
        if (error) {
            // yes
            return;
        }
        LOG_D(BRDR_TAG, "timeout");

        // no, we have timed out, so kill
        // the read operation
        // The read callback will be called
        // with an error
        port.cancel();
    }

public:

    // Constructs a blocking reader, pass in an open serial_port and
    // a timeout in milliseconds.
    blocking_reader(boost::asio::serial_port& port, size_t timeout) :
            port(port), timeout(timeout),
            timer( GET_IO_SERVICE(port) ),
            read_error(true) {

    }

    // Reads a character or times out
    // returns false if the read times out
    bool get_response(const char* request, size_t request_size, char& val) {

        val = c = '\0';

        // After a timeout & cancel it seems we need
        // to do a reset for subsequent reads to work.
        GET_IO_SERVICE(port).reset();

        // Asynchronously read 1 character.
        boost::asio::async_read(port, boost::asio::buffer(&c, 1),
                                boost::bind(&blocking_reader::read_complete,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));

        // send the request
        LOG_D(BRDR_TAG, "Sending request (size: " + std::to_string(request_size) + ")");
        port.write_some(boost::asio::buffer(request, request_size));

        // Setup a deadline time to implement our timeout.
        timer.expires_from_now(boost::posix_time::milliseconds(timeout));
        timer.async_wait(boost::bind(&blocking_reader::time_out,
                                     this, boost::asio::placeholders::error));

        // This will block until a character is read
        // or until the it is cancelled.
        GET_IO_SERVICE(port).run();

        if (!read_error)
            val = c;

        return !read_error;
    }
};


#endif //SELF_O_MAT_BLOCKING_READER_H
