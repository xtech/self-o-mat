#include <vector>
#include <logic/filters/IImageFilter.h>
#include <logic/filters/BasicImageFilter.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <string>
#include <opencv2/opencv.hpp>

using namespace selfomat::logic;
using namespace boost::filesystem;

int main(int argc, char *argv[]) {

    if(argc != 2) {
        std::cout << "Usage: ./filter_test <image in dir> <image out dir>" << std::endl;
        return 1;
    }




    std::vector<IImageFilter*> filters;
    filters.push_back(new BasicImageFilter());

    path p(argv[1]);
    directory_iterator end_itr;

    path out_dir("filter_test_output");

    system("rm -rf ./filter_test_output");

    create_directories(out_dir);


    struct timespec tstart, tend;

    // cycle through the directory
    int imageIndex = 0;
    for (directory_iterator itr(p); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path())) {
            clock_gettime(CLOCK_MONOTONIC, &tstart);

            imageIndex++;
            std::string current_file = itr->path().string();
            std::cout << current_file << std::endl;

            // Load file into image magick
            cv::Mat image;


            for(auto &filter : filters) {
                for(double d = 0.0; d <= 1.0; d+=0.25) {
                    clock_gettime(CLOCK_MONOTONIC, &tstart);
                    image = cv::imread(current_file);


                    double factor = 1182.0 / image.rows;
                    cv::resize(image, image, cv::Size(image.cols * factor, image.rows* factor));

                    clock_gettime(CLOCK_MONOTONIC, &tend);

                    printf("loading took %.5f s\n",
                           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
                    clock_gettime(CLOCK_MONOTONIC, &tstart);


//                    cv::resize(image, image, cv::Size(500,500));
                    cv::imwrite("scaled.jpg", image);
                    filter->processImage(image, d);
                    clock_gettime(CLOCK_MONOTONIC, &tend);

                    printf("filtering took %.5f s\n",
                           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

                    cv::imwrite("filtered.jpg", image);
                    std::string outfile =
                            out_dir.string() + "/" + std::to_string(imageIndex) + "_" + filter->getName() + "_" + std::to_string((int)(d*100)) +
                            itr->path().extension().string();
                    system((std::string("convert +append scaled.jpg filtered.jpg ") + outfile).c_str());
                    //system((std::string("mv filtered.jpg ")+outfile).c_str());
                }
            }
            clock_gettime(CLOCK_MONOTONIC, &tend);

            printf("filtering took %.5f s\n",
                   ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

        }
    }
}