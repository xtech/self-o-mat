cmake_minimum_required(VERSION 3.7)
project(self_o_mat)

set(CMAKE_CXX_STANDARD 14)

# Generate protobuf files on file change
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/api.pb.cc ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/api.pb.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/api.proto
        COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/compile_protobuf.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})



find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(Cups REQUIRED)
find_package(Pistache REQUIRED)
find_package(Protobuf REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

include_directories(/usr/local/include/gphoto2)
include_directories(/opt/libjpeg-turbo/include)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(${CUPS_INCLUDE_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${Protobuf_INCLUDE_DIR})
include_directories(/usr/local/include/pistache )


if(USE_SPI)
    message("We have wiring pi!")

    add_definitions(-DUSE_SPI)
else()
    message("We don't have wiring pi!")
endif()

add_executable(self_o_mat main.cpp BoothLogic.cpp BoothLogic.h BoothGui.cpp BoothGui.h ICamera.h OpenCVCamera.cpp OpenCVCamera.h GphotoCamera.cpp GphotoCamera.h FPSCounter.h NopCamera.h ImageProcessor.cpp ImageProcessor.h IGui.h ILogger.h ConsoleLogger.cpp ConsoleLogger.h buffers.h headers.cpp imageinfo.h PrinterManager.cpp PrinterManager.h BoothApi.h BoothApi.cpp protobuf/api.pb.cc protobuf/api.pb.h ICamera.cpp NopGui.cpp NopGui.h JpegDecoder.cpp JpegDecoder.h)

target_link_libraries(self_o_mat ${Boost_LIBRARIES})
target_link_libraries(self_o_mat gphoto2)
target_link_libraries(self_o_mat turbojpeg)
target_link_libraries(self_o_mat sfml-system)
target_link_libraries(self_o_mat sfml-graphics)
target_link_libraries(self_o_mat sfml-window)
target_link_libraries(self_o_mat ${OpenCV_LIBS})
target_link_libraries(self_o_mat ${ImageMagick_LIBRARIES})
target_link_libraries(self_o_mat ${CUPS_LIBRARIES})
target_link_libraries(self_o_mat pistache )
target_link_libraries(self_o_mat protobuf)

if(USE_SPI)
    target_link_libraries(self_o_mat wiringPi)
endif()



#add_dependencies(self_o_mat compile_protobuf)
