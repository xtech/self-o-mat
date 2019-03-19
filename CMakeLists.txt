cmake_minimum_required(VERSION 3.7)
project(self_o_mat)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=8)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

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

include_directories(src)
include_directories(dependencies)
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

add_executable(self_o_mat src/main.cpp src/logic/BoothLogic.cpp src/logic/BoothLogic.h src/ui/BoothGui.cpp src/ui/BoothGui.h
        src/camera/ICamera.h src/camera/OpenCVCamera.cpp src/camera/OpenCVCamera.h src/camera/GphotoCamera.cpp src/camera/GphotoCamera.h
        src/ui/FPSCounter.h src/camera/NopCamera.h src/logic/ImageProcessor.cpp src/logic/ImageProcessor.h src/ui/IGui.h src/tools/ILogger.h
        src/tools/ConsoleLogger.cpp src/tools/ConsoleLogger.h src/tools/buffers.h src/tools/buffers.cpp src/tools/imageinfo.h src/logic/PrinterManager.cpp
        src/logic/PrinterManager.h src/api/BoothApi.h src/api/BoothApi.cpp src/protobuf/api.pb.cc src/protobuf/api.pb.h src/ui/NopGui.cpp
        src/ui/NopGui.h src/tools/JpegDecoder.cpp src/tools/JpegDecoder.h src/tools/verbose.h
        src/camera/controllers/GphotoBaseController.h
        dependencies/easyexif/exif.h dependencies/easyexif/exif.cpp src/camera/controllers/GphotoTriggerController.cpp src/camera/controllers/GphotoTriggerController.h src/camera/controllers/GphotoBaseController.cpp src/camera/controllers/GphotoFocusController.cpp src/camera/controllers/GphotoFocusController.h src/camera/controllers/GphotoCameraInfoController.cpp src/camera/controllers/GphotoCameraInfoController.h)

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
