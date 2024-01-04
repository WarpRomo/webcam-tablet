cmake_minimum_required(VERSION 3.23)
project(HelloWorld)

set(CMAKE_CXX_STANDARD 23)
set(ENV{OPENCV_DIR} "C:\\tools\\opencv\\build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(HelloWorld main.cpp
        pixelFuncs.cpp
        pixelFuncs.h
        tabletCalc.cpp
        tabletCalc.h
        GUI/guiFuncs.cpp
        GUI/guiFuncs.h
        GUI/draggableGUI.cpp
        GUI/draggableGUI.h
        mouseFuncs.cpp
        mouseFuncs.h
        screenRes.cpp
        screenRes.h
        GUI/guiColors.cpp
        GUI/guiColors.h
)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

target_link_libraries(HelloWorld ${OpenCV_LIBS})
