cmake_minimum_required(VERSION 3.3)
project(Detect_RGBD)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(SOURCE_FILES
    CalDepthDifNormalVec.cpp
    CalDepthDifNormalVec.h
    COpenNi.cpp
    COpenNi.h
    Data3DHelper.h
    GetRect.cpp
    GetRect.h
    HeadDetect.cpp
    HeadDetect.h
    HeadFiles.h
    HeadFinder.cpp
    HeadFinder.h
    main.cpp
    MatHelper.h
    PreProcess.cpp
    PreProcess.h)

add_executable(Detect_RGBD ${SOURCE_FILES})

FIND_PACKAGE(OpenCV REQUIRED)
find_package(Boost 1.54 REQUIRED system serialization)
target_link_libraries(Detect_RGBD ${OpenCV_LIBS})