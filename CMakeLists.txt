cmake_minimum_required(VERSION 3.7)
project(self_o_mat)

set(CMAKE_CXX_STANDARD 14)

if(VERBOSE)
    message("Verbose is set, adding definition")
    add_definitions(-DVERBOSE)
endif()
add_definitions(-DVERBOSE)

# Generate protobuf files on file change
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf/api.pb.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf/api.pb.h
        DEPENDS api.proto
        COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/compile_protobuf.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})



find_package(OpenCV 4 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(Cups REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(src)
include_directories(dependencies)
include_directories(/usr/local/include/gphoto2)
include_directories(/opt/libjpeg-turbo/include)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CUPS_INCLUDE_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${Protobuf_INCLUDE_DIR})
include_directories(dependencies/served/src)

OPTION (SERVED_BUILD_TESTS "Build served unit test suite" OFF)
OPTION (SERVED_BUILD_EXAMPLES "Build served  examples" OFF)
add_subdirectory(dependencies/served)

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
        src/ui/NopGui.h src/tools/JpegDecoder.cpp src/tools/JpegDecoder.h src/tools/verbose.h src/tools/blocking_reader.h
        src/camera/controllers/BaseController.h src/camera/controllers/TriggerController.cpp
        src/camera/controllers/TriggerController.h src/camera/controllers/BaseController.cpp src/camera/controllers/FocusController.cpp
        src/camera/controllers/FocusController.h src/camera/controllers/InfoController.cpp src/camera/controllers/InfoController.h
        src/camera/controllers/ApertureController.h src/camera/controllers/ExposureCorrectionController.h src/camera/controllers/ChoiceRangeController.cpp
        src/camera/controllers/ChoiceRangeController.h src/tools/readfile.h src/tools/readfile.cpp src/tools/cobs.h src/tools/cobs.cpp src/logic/SelfomatController.cpp src/logic/SelfomatController.h src/logic/filters/IImageFilter.h src/logic/filters/BasicImageFilter.cpp src/logic/filters/BasicImageFilter.h src/logic/filters/IImageFilter.cpp)



target_link_libraries(self_o_mat ${Boost_LIBRARIES})
target_link_libraries(self_o_mat gphoto2)
target_link_libraries(self_o_mat turbojpeg)
target_link_libraries(self_o_mat sfml-system)
target_link_libraries(self_o_mat sfml-graphics)
target_link_libraries(self_o_mat sfml-window)
target_link_libraries(self_o_mat ${OpenCV_LIBS})
target_link_libraries(self_o_mat ${CUPS_LIBRARIES})
target_link_libraries(self_o_mat served)
target_link_libraries(self_o_mat protobuf)

add_executable(filter_test src/main_filter.cpp src/logic/filters/IImageFilter.h src/logic/filters/BasicImageFilter.cpp src/logic/filters/BasicImageFilter.h src/logic/filters/IImageFilter.cpp)
target_link_libraries(filter_test ${Boost_LIBRARIES})
target_link_libraries(filter_test ${OpenCV_LIBS})

add_executable(opencv_info src/main_opencv_info.cpp)
target_link_libraries(opencv_info ${OpenCV_LIBS})

if(USE_SPI)
    target_link_libraries(self_o_mat wiringPi)
endif()

add_custom_target(WITH_APP /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/deploy_app.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(PACK /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/pack.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(PACK self_o_mat)
