#ifndef GET_RECT_H
#define GET_RECT_H

//2000  //1650  // 1250
#define PosSamNO 128 //113 //77    //正样本个数  ;
#define NegSamNO 1330 //1103 //881   //负样本个数 ;
#define TRAIN true //是否进行训练,true表示重新训练，false表示读取xml文件中的SVM模型  ;
#define DescriptorDim 384 //192  //HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定;
//法向量//HOG
#include "COpenNi.h"
#include "HeadFiles.h"
#include "Data3DHelper.h"
#include "MatHelper.h"



#include "HOGFeather.h"

class GetRect
{
private:
    cv::Mat Pos; //正样本;
    cv::Mat Ign; //忽略样本;
    int TotalNum; //总共候选点的个数;
    //候选点的总数 = 正样本 + 负样本 +忽略样本;
    int PosNum; //正样本个数;
    int IgnNum; //负样本个数;
    
    CvSVM cvsvm;
    
    
public:
    GetRect();
    ~GetRect();
    
    void getrectimg(cv::Mat& m_oriBgrResultImage_8UC3, cv::Mat& m_preproResultImage_16UC1);
    
    
    
};



#endif