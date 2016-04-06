#pragma once
#ifndef HEAD_FINDER_H
#define HEAD_FINDER_H
#include "HeadFiles.h"
#include "Data3DHelper.h"
#include "MatHelper.h"

class HeadFinder
{
public:
    HeadFinder();
    ~HeadFinder();
    void findHeadPoints(const Mat &rgbFrame, const Mat &depthFrame, int curFrame); //简单获取头顶点
    void getHeadFeatures(vector<float> &feature, const Mat &depthFrame, const Mat &rgbFrame, int featureKind, Point2i headPoint);
    void drawRawHeadPoint(Mat &rgbFrame, const Mat &depthFrame); //头顶点处画框
    int calDepthDif(int x, int y, int i, Mat& depthFrame, int label);
    int calNormalVec(int px, int py, int x, int y, Mat& depthFrame, double& theta1, double& theta2);
private:
    Point2i* rawHeadPoints;
    //bool isTrain;
    int rawHeadPointsNum;
    int frame_num;
    FILE * fout,*fout2;
    string wFilePath;
    CvRTrees forest;
    int PosRow = -1, IgnRow = -1, PosCol, IgnCol;
    Mat Pos, Ign;
#define pi 3.1415926
#define TRAIN false
    bool draw_headpoint;
    clock_t start, end;
};

#endif