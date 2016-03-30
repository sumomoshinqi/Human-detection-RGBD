#ifndef HEAD_DETECT_H
#define HEAD_DETECT_H

#include "COpenNi.h"
//#include "HeadFiles.h"
#include "PreProcess.h"
//#include "Data3DHelper.h"
//#include "HOGFeature.h"
#include "MatHelper.h"
//#include "GetRect.h"
#include "HeadFinder.h"


class HeadDetect
{
private:
    
    cv::Mat m_oriBgrImage_8UC3; // 原始BGR图像
    cv::Mat m_oriHsvImage_8UC3;
    
    cv::Mat m_oriDepthImage_16UC1;
    cv::Mat m_oriDepthImage_8UC1;
    
    cv::Mat m_preproDepthImage_16UC1; // 预处理后的深度图像;
    cv::Mat m_preproDepthImage_8UC1;
    
public:
    HeadDetect();
    ~HeadDetect();
    
    void Run();
    
private:
    void UpdateFrame();
    void ShowResults();  
};

#endif