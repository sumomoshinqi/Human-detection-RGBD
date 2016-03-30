cmake_minimum_required(VERSION 3.3)
project(Graduation_thesis)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(SOURCE_FILES
    CalDepthDifNormalVec.cpp
    CalDepthDifNormalVec.h
    COpenNi.cpp
    COpenNi.h
    Data3DHelper.h
        main.cpp
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

add_executable(Graduation_thesis ${SOURCE_FILES})

FIND_PACKAGE(OpenCV REQUIRED)
target_link_libraries( Graduation_thesis ${OpenCV_LIBS} )