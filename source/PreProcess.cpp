#include "PreProcess.h"

void PreProcess::Denoise(cv::Mat& oriDepthImage_16UC1, cv::Mat& preproResultImage_16UC1, int iWinSize)
{
    oriDepthImage_16UC1.copyTo(preproResultImage_16UC1);
    
    int winSize = iWinSize;     // Window size, better with an odd number
    int halfWinSize = (winSize + 1) / 2 - 1;
    
    if (halfWinSize <= 0)
    {
        return;
    }
    
    // 将深度值为0的像素改为周围9*9区域内非0像素的均值，如果9*9区域内都为0，则不处理
    for (int i = halfWinSize; i < oriDepthImage_16UC1.rows - halfWinSize; i++) //先不考虑图片边缘的点
    {
        for (int j = halfWinSize; j < oriDepthImage_16UC1.cols - halfWinSize; j++)
        {
            if (oriDepthImage_16UC1.at<unsigned short>(i, j) == 0)//如果为0，寻找周围的非零点
            {
                long totalDepth = 0;	// 非零像素点的总深度
                int  numberOfPixel = 0;	// 非零的像素点的数目
                
                for (int nn = 1; nn <= halfWinSize; nn++)
                {
                    for (int kk = i - nn; kk <= i + nn; kk++)
                    {
                        for (int mm = j - nn; mm <= j + nn; mm++)
                        {
                            unsigned int depth_kk_mm = oriDepthImage_16UC1.at<unsigned short>(kk, mm);
                            
                            if (depth_kk_mm != 0)
                            {
                                numberOfPixel++;
                                totalDepth += depth_kk_mm;
                            }
                            
                        } // mm
                    } // kk
                    
                    if (numberOfPixel != 0)
                    {
                        preproResultImage_16UC1.at<unsigned short>(i, j) = totalDepth / numberOfPixel;
                        break;
                    }
                }
                
            }
        } // j
    } // i
    
}

void PreProcess::Do(cv::Mat& oriDepthImage_16UC1, cv::Mat& preproResultImage_16UC1)
{
    //oriDepthImage_16UC1.copyTo(preproResultImage_16UC1);
    
    //// 去噪
    //Denoise(oriDepthImage_16UC1, preproResultImage_16UC1, 3);
    
    // 中值滤波
    cv::medianBlur(oriDepthImage_16UC1, preproResultImage_16UC1, 5);
}

void PreProcess::DoAll(cv::Mat& oriDepthImage_16UC1)
{
    //对mat的黑洞进行填补，直到没有黑洞为止;
    int blacknum, countnum = 0;
    do{
        blacknum = 0;
        Denoise(oriDepthImage_16UC1, oriDepthImage_16UC1, 3);
        cv::medianBlur(oriDepthImage_16UC1, oriDepthImage_16UC1, 5);
        
        for (int j = 0; j < oriDepthImage_16UC1.rows; j++)
        {
            for (int i = 0; i < oriDepthImage_16UC1.cols; i++)
            {
                if (oriDepthImage_16UC1.at<unsigned short>(j, i) == 0)
                {
                    blacknum++;
                }
            }//for i
        }//for j
        
        //printf("balcknum = %d \n",blacknum);
        countnum++;
        
    } while (blacknum != 0 && countnum < 100);
    
}