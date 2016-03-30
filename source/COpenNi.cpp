//#include "stdafx.h"
#include "COpenNi.h"
#define DEPTH_DIR "/home/sumomoshinqi/Develop/CV/RGBD/38/depth/depth_"
#define RGB_DIR "/home/sumomoshinqi/Develop/CV/RGBD/38/rgb/rgb_"

COpenNI::COpenNI()
{
    
}

COpenNI::~COpenNI()
{
    //m_Context.Release();
}

int COpenNI::getCurFrameNum() const
{
    return  curFrameNum;
}

int COpenNI::getTotalFrameNum() const
{
    return FrameNum;
}

void COpenNI::setFileStorage()
{
//    string dFileName = "depth_" + to_string(curFrameNum);
    //dFilePath = "G:\\实验室\\陈悠12.11\\LabDataSample\\Depth\\" + dFileName + ".png";
    //dFilePath = "F:\\dataset\\1\\depth\\png\\1 (" + to_string(curFrameNum) + ")_8UC1_From1File.png";
    //dFilePath = "F:\\dataset\\38\\depth\\depth_" + to_string(curFrameNum) + ".png";
    dFilePath = DEPTH_DIR + to_string(curFrameNum) + ".png";
    //string rFileName = "rgb_" + to_string(curFrameNum);
    //rFilePath = "G:\\实验室\\陈悠12.11\\LabDataSample\\RGB\\" + rFileName + ".png";
    //rFilePath = "F:\\dataset\\38\\rgb\\rgb_" + to_string(curFrameNum) + ".png";
    rFilePath = RGB_DIR + to_string(curFrameNum) + ".png";
}


/* Update , Get new data */
bool COpenNI::UpdateData()
{
    try
    {
        //dFile[dFileName] >> curDepthFrame;
        //rFile[rFileName] >> curRGBFrame;
        curDepthFrame = imread(dFilePath, CV_LOAD_IMAGE_ANYDEPTH);
        curRGBFrame = imread(rFilePath);
        curFrameNum++;
        setFileStorage();
        return true;
    }
    catch (cv::Exception &e)
    {
        const char * s_ERROR = e.what();
        cout << s_ERROR << endl;
        return false;
    }
}
