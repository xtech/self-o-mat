cmake_minimum_required(VERSION 3.7)
project(self_o_mat)

set(CMAKE_CXX_STANDARD 14)

if(VERBOSE)
    message("Verbose is set, adding definition")
    add_definitions(-DVERBOSE)
endif()

# Generate protobuf files on file change
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}src/protobuf/api.pb.cc ${CMAKE_CURRENT_SOURCE_DIR}src/protobuf/api.pb.h
        DEPENDS api.proto
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

add_executable(self_o_mat src/main.cpp src/BoothLogic.cpp src/BoothLogic.h src/BoothGui.cpp src/BoothGui.h
        src/ICamera.h src/OpenCVCamera.cpp src/OpenCVCamera.h src/GphotoCamera.cpp src/GphotoCamera.h
        src/FPSCounter.h src/NopCamera.h src/ImageProcessor.cpp src/ImageProcessor.h src/IGui.h src/ILogger.h
        src/ConsoleLogger.cpp src/ConsoleLogger.h src/buffers.h src/headers.cpp src/imageinfo.h src/PrinterManager.cpp
        src/PrinterManager.h src/BoothApi.h src/BoothApi.cpp src/protobuf/api.pb.cc src/protobuf/api.pb.h src/ICamera.cpp src/NopGui.cpp
        src/NopGui.h src/JpegDecoder.cpp src/JpegDecoder.h src/verbose.h
        src/IGphotoController.h src/CanonShutterButtonCameraController.cpp src/CanonShutterButtonCameraController.h
        src/BasicCameraController.cpp src/BasicCameraController.h
        dependencies/easyexif/exif.h dependencies/easyexif/exif.cpp)

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
