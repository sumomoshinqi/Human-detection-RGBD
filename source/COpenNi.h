#ifndef C_OPEN_NI_H
#define C_OPEN_NI_H

#include "HeadFiles.h"
#include "MatHelper.h"
/* Class for control OpenNI device */
class COpenNI
{
public:
    /* Destructor */
    COpenNI();
    ~COpenNI();
    COpenNI(int tfNum) : FrameNum(tfNum)
    {
        curFrameNum = 2000;											//2015.3.26修改，之前是1
        curDepthFrame = cv::Mat(480, 640, CV_16UC1);
        curRGBFrame = cv::Mat(480, 640, CV_8UC3);
        setFileStorage();
    }
    
    int getCurFrameNum() const;
    
    int getTotalFrameNum() const;
    
    Mat getDepthFrame() const;
    
    Mat getRGBFrame() const;
    
    /* Update / Get new data */
    bool UpdateData();
    Mat curDepthFrame;
    Mat curRGBFrame;
    
private:
    void setFileStorage();
    int FrameNum;
    int curFrameNum;
    //FileStorage dFile;
    //FileStorage rFile;
    
    string dFilePath;
    string rFilePath;
};


#endif



//Mat data(100, 20, CV_32F);
//signed short pre_depth;
//int p, count, i;
//int ordinate_x, ordinate_y, aft_x, aft_y;
//signed short depth_dif[21];
//for (i = 0; i < rawHeadPointsNum; i++)
//{
//	int x = rawHeadPoints[i].x; //对应之前的i
//	int y = rawHeadPoints[i].y; //对应之前的j
//	int pixelNum = Data3DHelper::GetSizeInImageBySizeIn3D(150, depthFrame.at<unsigned short>(y, x));
//
//	if (int(x - 1.5*pixelNum - 1) < 0 || int(x + 1.5*pixelNum + 1) > 640 || int(y + 3.3*pixelNum + 1) > 480 || int(y - 0.7*pixelNum - 1) < 0)
//	{
//		continue;
//	}
//
//	count = 0;
//
//	ordinate_x = x;
//	ordinate_y = y;
//	pre_depth = depthFrame.at<unsigned short>(ordinate_y, ordinate_x);
//	/*r++;*/
//
//	for (p = 0; p < 7; p++)//IIIqu，选取了7个area
//	{
//		aft_x = ordinate_x + Data3DHelper::GetSizeInImageBySizeIn3D(18 * cos(p * pi / 6), pre_depth);
//		aft_y = ordinate_y + Data3DHelper::GetSizeInImageBySizeIn3D(7, pre_depth) - Data3DHelper::GetSizeInImageBySizeIn3D(18 * sin(p * pi / 6), pre_depth);
//		//ordinate_x,ordinate_y都是头顶点P的坐标，而算周围的相邻点的时候都是用的头的中心点坐标，故aft_y要加上一个头的半径所对应的像素点数
//		data.at<float>(i, count) = depthFrame.at<unsigned short>(aft_y, aft_x) - pre_depth;
//		//计算属于哪个区域的时候都是用周围像素点的深度减去头顶点P的深度
//		depth_dif[count] = depthFrame.at<unsigned short>(aft_y, aft_x) - pre_depth;
//		count++;
//	}