#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H

#include "HeadFiles.h"

class PreProcess
{
public:
    // Noise reduction
    static void Denoise(cv::Mat& oriDepthImage_16UC1, cv::Mat& preproResultImage_16UC1, int iWinSize);
    static void Do(cv::Mat& oriDepthImage_16UC1, cv::Mat& preproResultImage_16UC1);
    static void DoAll(cv::Mat& oriDepthImage_16UC1);
};

#endif