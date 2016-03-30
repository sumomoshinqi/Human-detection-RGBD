#include "HeadDetect.h"
#include "HeadFinder.h"

COpenNI g_openNi = COpenNI(2501);

HeadDetect::HeadDetect()
{
    
    m_oriBgrImage_8UC3 = cv::Mat(480, 640, CV_8UC3);
    m_oriHsvImage_8UC3 = cv::Mat(480, 640, CV_8UC3);
    
    m_oriDepthImage_16UC1 = cv::Mat(480, 640, CV_8UC1);
    m_oriDepthImage_8UC1 = cv::Mat(480, 640, CV_8UC1);
    
    m_preproDepthImage_16UC1 = cv::Mat(480, 640, CV_8UC1);
    m_preproDepthImage_8UC1 = cv::Mat(480, 640, CV_8UC1);
    
}

HeadDetect::~HeadDetect()
{
}

void HeadDetect::Run()
{
    char cKey = 0;
    HeadFinder hf = HeadFinder();
    while (cKey != 27) // 27是ESC键
    {
        if (g_openNi.getCurFrameNum() < g_openNi.getTotalFrameNum()){
            UpdateFrame();
            //getchar();
            hf.findHeadPoints(m_oriBgrImage_8UC3, m_preproDepthImage_16UC1, g_openNi.getCurFrameNum());
            hf.drawRawHeadPoint(m_oriBgrImage_8UC3, m_preproDepthImage_16UC1);
            ShowResults();
            cKey = waitKey(1);
        }
        else
            break;
    }
}

void HeadDetect::UpdateFrame()
{
    // 将kinect的RGB数据拷贝至oriRgbImage_8UC3
    // memcpy(m_oriBgrImage_8UC3.data, g_openNi.m_ImageMD.Data(), 480*640*3); // 存入的是Kinect的RGB信息 OpenNI Feature
    // cv::cvtColor(m_oriBgrImage_8UC3, m_oriBgrImage_8UC3, CV_RGB2BGR); // RGB转换为BGR
    // 将kinect的深度数据拷贝至oriDepthImage_16UC1
    // memcpy(m_oriDepthImage_16UC1.data, g_openNi.m_DepthMD.Data(), 480*640*2); //OpenNI Feature
    // 得到HSV图像
    // cv::cvtColor(m_oriBgrImage_8UC3, m_oriHsvImage_8UC3, CV_BGR2HSV); // BGR转换为HSV
    // if (g_openNi.m_ImageGenerator.GetFrameID() % 50 == 0){
    //
    //
    //
    //         char s[100];
    //         sprintf(s,"F:\\result\\head hog\\img_%d.jpg",g_openNi.m_ImageGenerator.GetFrameID());
    //         IplImage imageShow;
    //         imageShow = IplImage(m_oriBgrImage_8UC3);
    //         cvSaveImage(s,&imageShow);
    //
    //    }
    
    if (!g_openNi.UpdateData())
        exit(-1);
    m_oriDepthImage_16UC1 = g_openNi.curDepthFrame;
    m_oriBgrImage_8UC3 = g_openNi.curRGBFrame;
    cout << "Frame: " << g_openNi.getCurFrameNum() - 1 << " Loaded!" << endl;
    // 预处理;
    PreProcess::Do(m_oriDepthImage_16UC1, m_preproDepthImage_16UC1);
    //  hod H;
    // H.getHod(m_oriBgrImage_8UC3,m_preproDepthImage_16UC1);//传入深度图和RGB图
    
    //if ((g_openNi.getCurFrameNum() - 1) % 50 == 1)										//更改2015.3.24
    //{
    //	getchar();
    //}
    /*ShowResults();*/
    //if ((g_openNi.getCurFrameNum() - 1) == 50)
    //{
    //	HeadFinder fs = HeadFinder();
    //	fs.findHeadPoints(m_oriBgrImage_8UC3, m_preproDepthImage_16UC1, g_openNi.getCurFrameNum() - 1);
    //	//fs.drawGroundTruth(m_oriBgrImage_8UC3, m_preproDepthImage_16UC1);
    //	ShowResults();
    //	getchar();
    //}
    //if (g_openNi.getCurFrameNum() - 1 == 50)
    
    // 显示;
    
}

void HeadDetect::ShowResults()
{
    // 显示原始深度图像;
    //	{
    // 		m_oriDepthImage_16UC1.convertTo(m_oriDepthImage_8UC1, CV_8U, 255.0/7096.0); // 16位数据转换为8位数据，同时进行尺度缩放;
    // 		cv::imshow("原始深度图像", m_oriDepthImage_8UC1);
    //
    // 	}
    
    // 显示预处理后的深度图像;
    //{
    //
    //	m_preproDepthImage_16UC1.convertTo(m_preproDepthImage_8UC1, CV_8U , 255.0/7096.0); // 16位数据转换为8位数据，同时进行尺度缩放;
    //	cv::imshow("预处理后的深度图;", m_preproDepthImage_8UC1);
    //
    //}
    
    // 显示原始BGR图像
    {
        char buf1[80];
        //sprintf(buf1,"Frame: %d",g_openNi.m_ImageGenerator.GetFrameID());
        sprintf(buf1, "Frame: %d", g_openNi.getCurFrameNum() - 1);
        cv::putText(m_oriBgrImage_8UC3, buf1, cvPoint(25, 25), CV_FONT_HERSHEY_DUPLEX, 1, CV_RGB(0, 20, 0));
        
        cv::imshow("Original RGB Image", m_oriBgrImage_8UC3);
        //cout << g_openNi.getCurFrameNum() - 1 << endl;
        //waitKey();
    }
}

