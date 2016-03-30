#ifndef Cal_Depth_Dif_Normal_Vec_H
#define Cal_Depth_Dif_Normal_Vec_H

#include "HeadFiles.h"
#include "HeadFinder.h"

class CalDepthDifNormalVec
{
public:
	CalDepthDifNormalVec();
	~CalDepthDifNormalVec();

	int calDepthDif(int x, int y, int pixelNum, Mat& depthFrame);
	int calNormalVec();
};

#endif