#ifndef DATA_3D_HELPER_H
#define DATA_3D_HELPER_H

#include "HeadFiles.h"
#include "HeadDetect.h"


class Data3DHelper
{
public:
    static int GetSizeInImageBySizeIn3D(const int iSizeIn3D, const int iDistance)
    {
        if (iDistance == 0 || iSizeIn3D == 0)
        {
            return 0;
        }
        
        static double dConstFactor = 0.0; // 常数，表示空间中？长度的距离相机为1毫米的直线将投影成一个像素宽
        static bool bIsFactorComputed = false; // 因为只想在第一次计算一次常数，以后不再计算，本变量保存计算状态
        
        if (!bIsFactorComputed)
        {
            // 在深度图上假定两点
            //pPoint3D[2] = { { 0, 0, 1000 }, { 100, 0, 1000 } }; // {列, 行, 深度}
            //float X1, X2, Y1, Y2, D1, D2;
            //float rX1, rX2, rY1, rY2, rZ1, rZ2;
            //X1 = 0; //1的列
            //Y1 = 0; //1的行
            //D1 = 1000;
            //X2 = 100; //2的列
            //Y2 = 0; //2的行
            //D2 = 1000;
            //g_openNi.m_DepthGenerator.ConvertProjectiveToRealWorld(2, pPoint3D, pPoint3D); // 可以得到点在空间中的实际坐标
            //CoordinateConverter::convertDepthToWorld(g_openNi.streamDepth, X1, Y1, D1, &rX1, &rY1, &rZ1);
            //CoordinateConverter::convertDepthToWorld(g_openNi.streamDepth, X2, Y2, D2, &rX2, &rY2, &rZ2);
            // 两点在空间中的实际距离
            double d3DDistance = 173.66668978426750;//std::sqrt((long double)((rX1 - rX2) * (rX1 - rX2) + (rY1 - rY2) * (rY1 - rY2) + (rZ1 - rZ2) * (rZ1 - rZ2)));
            dConstFactor = d3DDistance / (1000 * 100);
            bIsFactorComputed = true;
        }
        
        return (int)(((double)iSizeIn3D / (double)iDistance) / dConstFactor);
        
    }
    
};

#endif