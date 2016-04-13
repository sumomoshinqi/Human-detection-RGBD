#include "HeadFinder.h"
#include "COpenNi.h"

extern COpenNI g_openNi;


HeadFinder::HeadFinder()
{
    //isTrain = _isTrain;
    rawHeadPoints = new Point2i[640 * 480];
    rawHeadPointsNum = 0;
    frame_num = 0;
    fout = fopen("/home/sumomoshinqi/Develop/CV/Human-detection-RGBD/source/47_label/Korean_47_50800-52200_train_data_normal.txt", "a");//以追加方式写进txt文档要用“ab”，a代表追加，b代表二进制文本
    //forest.load("per10-hou3500.xml");

//    fout2 = fopen("/home/sumomoshinqi/Develop/CV/Human-detection-RGBD/source/train.txt","a");


    Pos = Mat(141, 7, CV_32FC1);
    Ign = Mat(141, 5, CV_32FC1);

    // Positive features
    MatHelper::ReadMat("32F", Pos, "/home/sumomoshinqi/Develop/CV/Human-detection-RGBD/source/47_label/Pos_47_50800-52200.txt");
    // Ignored features
    MatHelper::ReadMat("32F", Ign, "/home/sumomoshinqi/Develop/CV/Human-detection-RGBD/source/47_label/Ign_47_50800-52200.txt");

    forest.load("/home/sumomoshinqi/Develop/CV/Human-detection-RGBD/source/util/cloth_38_47.xml");

}


HeadFinder::~HeadFinder()
{
}



void HeadFinder::findHeadPoints(const Mat &rgbFrame, const Mat &depthFrame, int curFrame)
{
    start = clock();
    //Reset rawHeadPoints
    rawHeadPointsNum = 0;
    delete[] rawHeadPoints;
    rawHeadPoints = new Point2i[640 * 480];
    //Find head points
    Mat tempRGB;
    rgbFrame.copyTo(tempRGB);
    //cout << depthFrame.at<int>(100, 100) << endl;
    for (int j =50; j < depthFrame.rows; ++j)//j=50
    {
        //circle(tempRGB, Point(540, j), 1, Scalar(255, 255, 255), -1, 8); //标记出不要的部分;

        for (int i = 1; i < depthFrame.cols; ++i)
        {
            if (depthFrame.at<int>(j, i) > 800 && depthFrame.at<int>(j, i) < 7000 //深度值要合理//800,6000
                && abs(depthFrame.at<int>(j, i - 1) - depthFrame.at<int>(j, i)) > 200) //左右两个相邻点有深度差//////改前为200 2015.6.11修改
            {
                bool isHeadPoint = 1;
                int pixelNum = Data3DHelper::GetSizeInImageBySizeIn3D(150, depthFrame.at<int>(j, i));
                for (int k = pixelNum; k > -pixelNum && i + k > 0 && i + k < 640; k--)
                {
                    if (abs(depthFrame.at<int>(j, i) - depthFrame.at<int>(j - 1, i + k)) < 180)//上一行画一条线，长为2r，要这条线上深度都很小，那么就认为是头顶点的切线（我的理解）////改前为180 2015.6.11修改
                    {
                        //abs是因为背景可能为0
                        isHeadPoint = 0;//不是顶点;
                        break;
                    }
                }
                if (isHeadPoint)
                {   
                    bool isMarked = 1;
                    for (int lj = -2; lj < 1; lj++)
                    {
                        for (int li = -1; li > -pixelNum && i + li > 0 && i + li < 640; li--)
                        {
                            if (tempRGB.at<Vec3b>(j + lj, i + li)[0] == 0 && tempRGB.at<Vec3b>(j + lj, i + li)[1] == 255 && tempRGB.at<Vec3b>(j + lj, i + li)[2] == 255)
                            {
                                isMarked = 0;//之前已经标记过点了; 因为从左往右扫描，于是判断左边i-1到i+li长，高j-2到j这个范围有没被标记过
                                break;
                            }
                        }//for li
                        if (!isMarked)
                            break;
                    }//for lj
                    if (isMarked)
                    {
                        circle(tempRGB, cv::Point(i, j), 3, cv::Scalar(0, 255, 255), -1, 8);//黄色;
                        if (int(i - 1.5*pixelNum - 1) < 0 || int(i + 1.5*pixelNum + 1) > 640 || int(j + 3.3*pixelNum + 1) > 480 || int(j - 0.7*pixelNum - 1) < 0)
                        {
                            //把框取不到的情况忽略
                        }
                        else
                            rawHeadPoints[rawHeadPointsNum++] = Point2i(i, j);
                    }
                    else
                        isHeadPoint = 0;
                }
                else
                {
                    continue;
                }

            }
        }
    }
}

void HeadFinder::getHeadFeatures(vector<float> &feature, const Mat &depthFrame, const Mat &rgbFrame, int featureKind, Point2i headPoint) //Should Mat be released?
{
    int pixelNum = Data3DHelper::GetSizeInImageBySizeIn3D(150, depthFrame.at<int>(headPoint.y, headPoint.x));
    Mat RectImg, resizedImage;
    HOGDescriptor hog(cvSize(48, 64), cvSize(32, 32), cvSize(16, 16), cvSize(16, 16), 8);// 分别是 WinSize BlockSize BlockStride cellSize nBins
    Mat tempRGB, tempDepth;
    rgbFrame.copyTo(tempRGB);
    depthFrame.copyTo(depthFrame);
    switch (featureKind)
    {
        case 0: //HoG Feature
        {
            RectImg.create(4 * pixelNum + 2, 3 * pixelNum + 2, CV_8UC3);
            MatHelper::GetRectMat(tempRGB, RectImg, int(headPoint.x - 1.5*pixelNum - 1), int(headPoint.y - 0.7*pixelNum - 1), 3 * pixelNum + 2, 4 * pixelNum + 2);//width height
            resize(RectImg, resizedImage, cv::Size(48, 64));
            hog.compute(resizedImage, feature, cv::Size(16, 16));
            RectImg.release();
            resizedImage.release();
            break;
        }
        case 1: //HoD Feature
        {
            Mat resizedImage_8UC1;
            RectImg.create(4 * pixelNum + 2, 3 * pixelNum + 2, CV_8UC3);
            MatHelper::GetRectMat(tempDepth, RectImg, int(headPoint.x - 1.5*pixelNum - 1), int(headPoint.y - 0.7*pixelNum - 1), 3 * pixelNum + 2, 4 * pixelNum + 2);//width height
            // resizedDImage.create(64, 48, CV_16UC1);
            resize(RectImg, resizedImage, cv::Size(48, 64));
            resizedImage.convertTo(resizedImage_8UC1, CV_8U, 255.0 / 7096.0);
            hog.compute(resizedImage_8UC1, feature, cv::Size(16, 16), cv::Size(0, 0));
            RectImg.release();
            resizedImage.release();
            resizedImage_8UC1.release();
            break;
        }
        default:
        {
            cout << "Wrong Feature Kind" << endl;
            return;
        }
    }


}

void HeadFinder::drawRawHeadPoint(Mat &rgbFrame, const Mat &depthFrame)//画头顶点、画框和标记头顶点序号
{
    //for (int i = 0; i < rawHeadPointsNum; i++)
    //{
    //	int x = rawHeadPoints[i].x; //对应之前的i
    //	int y = rawHeadPoints[i].y; //对应之前的j
    //	int pixelNum = Data3DHelper::GetSizeInImageBySizeIn3D(150, depthFrame.at<int>(y, x));

    //	/*	if (int(x - 1.5*pixelNum - 1) < 0 || int(x + 1.5*pixelNum + 1) > 640 || int(y + 3.3*pixelNum + 1) > 480 || int(y - 0.7*pixelNum - 1) < 0)
    //	{
    //	continue;
    //	}*/

    //	{
    //		circle(rgbFrame, Point(x, y), 3, Scalar(0, 255, 255), -1, 8);
    //		rectangle(rgbFrame, cv::Rect(x - 1.5*pixelNum, y - 0.7*pixelNum, 3 * pixelNum, 4 * pixelNum), cv::Scalar(0, 0, 255), 1, 8);

    //		putText(rgbFrame, to_string(i + 1), cvPoint(x, y), CV_FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(0, 255, 0)); //绿色;
    //	}
    //	if (((g_openNi.getCurFrameNum() - 1)) % 10 == 0)
    //	{
    //		wFilePath = "F:\\paper_demo\\detect_people_v1\\detect_people_v1\\June11_LiuJun\\220\\220_" + to_string(g_openNi.getCurFrameNum() - 1) + ".png";
    //		imwrite(wFilePath, rgbFrame);
    //	}
    //}

    if (TRAIN)//获得训练xml的数据data
    {
        if ((g_openNi.getCurFrameNum() - 1) % 10 == 0)
        {
            PosRow++;
            PosCol = 0;
            IgnRow++;
            IgnCol = 0;
        }

        for (int i = 0; i < rawHeadPointsNum; i++)
        {
            int x = rawHeadPoints[i].x; //对应之前的i
            int y = rawHeadPoints[i].y; //对应之前的j
            int pixelNum = Data3DHelper::GetSizeInImageBySizeIn3D(150, depthFrame.at<int>(y, x));

            /*	if (int(x - 1.5*pixelNum - 1) < 0 || int(x + 1.5*pixelNum + 1) > 640 || int(y + 3.3*pixelNum + 1) > 480 || int(y - 0.7*pixelNum - 1) < 0)
             {
             continue;
             }*/

            {
                circle(rgbFrame, Point(x, y), 3, Scalar(0, 255, 255), -1, 8);
                rectangle(rgbFrame, cv::Rect(x - 1.5*pixelNum, y - 0.7*pixelNum, 3 * pixelNum, 4 * pixelNum), cv::Scalar(0, 0, 255), 1, 8);

                putText(rgbFrame, to_string(i + 1), cvPoint(x, y), CV_FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(0, 255, 0)); //绿色;
            }

            end = clock();
            double duration = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(fout2, "%f\n", duration);


            //if ((g_openNi.getCurFrameNum() - 1) % 10 == 0 /*&& (g_openNi.getCurFrameNum() - 1) <= 2000*/)
            //{
            //	if (i + 1 == Ign.at<float>(IgnRow, IgnCol))
            //	{
            //		IgnCol++;
            //		continue;
            //	}
            //	else if (i + 1 == Pos.at<float>(PosRow, PosCol))
            //	{
            //		PosCol++;
            //		Mat depthFrameClone(depthFrame);

            //		calDepthDif(x, y, i, depthFrameClone, 1);
            //	}
            //	else if ((i + 1) % 5 == 0)
            //	{
            //		//fprintf(fout, "%d\n", g_openNi.getCurFrameNum() - 1);
            //		//cout << g_openNi.getCurFrameNum() - 1 << endl; cout << i + 1 << endl;

            //		Mat depthFrameClone(depthFrame);

            //		calDepthDif(x, y, i, depthFrameClone, -1);
            //	}
            //}

        }
    }
    else//如果不训练，直接利用已生成的xml进行判断
    {
        Mat depthFrameClone(depthFrame);
        for (int i = 0; i < rawHeadPointsNum; i++)
        {
            draw_headpoint = false;
            int x = rawHeadPoints[i].x; //对应之前的i
            int y = rawHeadPoints[i].y; //对应之前的j
            int pixelNum = Data3DHelper::GetSizeInImageBySizeIn3D(150, depthFrame.at<int>(y, x));

            calDepthDif(x, y, i, depthFrameClone, -1);

            if (draw_headpoint)
            {
                circle(rgbFrame, Point(x, y), 3, Scalar(0, 255, 255), -1, 8);
                rectangle(rgbFrame, cv::Rect(x - 1.5*pixelNum, y - 0.7*pixelNum, 3 * pixelNum, 4 * pixelNum), cv::Scalar(0, 0, 255), 1, 8);

                putText(rgbFrame, to_string(i + 1), cvPoint(x, y), CV_FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(0, 255, 0)); //绿色;
                /*if (((g_openNi.getCurFrameNum() - 1) - 5) % 10 == 0)
                 {
                 wFilePath = "F:\\paper_demo\\detect_people_v1\\detect_people_v1\\cloth_38_47_normal\\38_normal_result_predictprob0.3\\38_normal_result_prepro0.3_" + to_string(g_openNi.getCurFrameNum()-1) + ".png";
                 imwrite(wFilePath, rgbFrame);
                 }*/
            }
        }
    }

}

int HeadFinder::calDepthDif(int x, int y, int i, Mat& depthFrame, int label)
{
    Mat data(1000, 60, CV_32F);
    signed short pre_depth;
    int p, count;
    int ordinate_x, ordinate_y, aft_x, aft_y;
    signed short depth_dif[21];
    double normalVec[60][2];
    Mat sample;
    float result;
    Mat depthFrameClone2(depthFrame);

    //for (i = 0; i < rawHeadPointsNum; i++)
    //{
    //	int x = rawHeadPoints[i].x; //对应之前的i
    //	int y = rawHeadPoints[i].y; //对应之前的j
    //	int pixelNum = Data3DHelper::GetSizeInImageBySizeIn3D(150, depthFrame.at<int>(y, x));

    //	if (int(x - 1.5*pixelNum - 1) < 0 || int(x + 1.5*pixelNum + 1) > 640 || int(y + 3.3*pixelNum + 1) > 480 || int(y - 0.7*pixelNum - 1) < 0)
    //	{
    //		continue;
    //	}

    count = 0;

    ordinate_x = x;
    ordinate_y = y;
    pre_depth = depthFrame.at<int>(ordinate_y, ordinate_x);



    for (p = 0; p < 7; p++)//IIIqu，选取了7个area
    {
        aft_x = ordinate_x + Data3DHelper::GetSizeInImageBySizeIn3D(18 * cos(p * pi / 6), pre_depth);
        aft_y = ordinate_y + Data3DHelper::GetSizeInImageBySizeIn3D(7, pre_depth) - Data3DHelper::GetSizeInImageBySizeIn3D(18 * sin(p * pi / 6), pre_depth);
        //ordinate_x,ordinate_y都是头顶点P的坐标，而算周围的相邻点的时候都是用的头的中心点坐标，故aft_y要加上一个头的半径所对应的像素点数
        data.at<float>(i, count) = depthFrame.at<int>(aft_y, aft_x) - pre_depth;

        //计算属于哪个区域的时候都是用周围像素点的深度减去头顶点P的深度
        depth_dif[count] = depthFrame.at<int>(aft_y, aft_x) - pre_depth;

        //Calculate normal vector
        calNormalVec(ordinate_x, ordinate_y, aft_x, aft_y, depthFrameClone2, normalVec[count][0], normalVec[count][1]);
        count++;
    }

    for (p = 0; p < 5; p++)//IIqu，选取了5个area
    {
        aft_x = ordinate_x + Data3DHelper::GetSizeInImageBySizeIn3D(18 * cos(-pi / 4 - p*pi / 8), pre_depth);
        aft_y = ordinate_y + Data3DHelper::GetSizeInImageBySizeIn3D(7, pre_depth) - Data3DHelper::GetSizeInImageBySizeIn3D(18 * sin(-pi / 4 - p*pi / 8), pre_depth);
        data.at<float>(i, count) = depthFrame.at<int>(aft_y, aft_x) - pre_depth;

        depth_dif[count] = depthFrame.at<int>(aft_y, aft_x) - pre_depth;

        //Calculate normal vector
        calNormalVec(ordinate_x, ordinate_y, aft_x, aft_y, depthFrameClone2, normalVec[count][0], normalVec[count][1]);
        count++;
    }

    for (p = 0; p < 8; p++)//Iqu,有过个area，此次选取了360°的8个。
    {
        aft_x = ordinate_x + Data3DHelper::GetSizeInImageBySizeIn3D(7 * cos(p * pi / 4), pre_depth);//7 * cos(p * pi / 4)
        aft_y = ordinate_y + Data3DHelper::GetSizeInImageBySizeIn3D(7, pre_depth) - Data3DHelper::GetSizeInImageBySizeIn3D(7 * sin(p * pi / 4), pre_depth);
        data.at<float>(i, count) = depthFrame.at<int>(aft_y, aft_x) - pre_depth;

        depth_dif[count] = depthFrame.at<int>(aft_y, aft_x) - pre_depth;

        //Calculate normal vector
        calNormalVec(ordinate_x, ordinate_y, aft_x, aft_y, depthFrameClone2, normalVec[count][0], normalVec[count][1]);
        count++;
    }

    if (TRAIN)//获得训练xml的数据data
    {
        //fprintf(fout,"Frame:%d\n",g_openNi.getCurFrameNum()-1);
        //fprintf(fout, "%d,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd",
        //	label, depth_dif[0], depth_dif[1], depth_dif[2], depth_dif[3], depth_dif[4], depth_dif[5],
        //	depth_dif[6], depth_dif[7], depth_dif[8], depth_dif[9], depth_dif[10], depth_dif[11],
        //	depth_dif[12], depth_dif[13], depth_dif[14], depth_dif[15], depth_dif[16], depth_dif[17],
        //	depth_dif[18], depth_dif[19]/*, depth_dif[20], depth_dif[21], depth_dif[22], depth_dif[23], depth_dif[24], depth_dif[25], depth_dif[26]*/);

        /*fprintf(fout, "%d", label);

         for (p = 0; p < 20; p++)
         fprintf(fout, ",%.2lf,%.2lf", normalVec[p][0],normalVec[p][1]);

         fprintf(fout, "\n");*/
    }
    else // 如果不训练，直接利用已生成的xml进行判断
    {
        //int sign = 0;
        for (p = 0; p < 20; p++)
        {
            data.at<float>(i, count) = normalVec[p][0];
            count++;
            data.at<float>(i, count) = normalVec[p][1];
            count++;
        }
        sample = data.row(i);
        //MatHelper::PrintMat("32F", sample, "D://2.txt");
        //cout << data.at<signed short>(0, 0) << endl;
        //cout << data.at<signed short>(0, 39) << endl;

        //result = forest.predict(sample);
        result = forest.predict_prob(sample);


        //cout << (end - start) / CLOCKS_PER_SEC *1000 << endl;
        //cout << result << endl;

        //cout << result << endl;
        if (result >0.7 /*result == 1*/ /*|| (int)result == 49*/)
            draw_headpoint = true;

    }

    return 0;
}


int HeadFinder::calNormalVec(int px, int py, int x, int y, Mat& depthFrame, double& theta1, double& theta2 )
{
    /*signed short Gradient_X,Gradient_Y,Gradient_PX,Gradient_PY;

     Gradient_X = 0.5*(depthFrame.at<int>(y, x + 1) - depthFrame.at<int>(y, x - 1));
     Gradient_Y = 0.5*(depthFrame.at<int>(y + 1, x) - depthFrame.at<int>(y - 1, x));

     Gradient_PX = 0.5*(depthFrame.at<int>(py, px + 1) - depthFrame.at<int>(py, px - 1));
     Gradient_PY = 0.5*(depthFrame.at<int>(py + 1, px) - depthFrame.at<int>(py - 1, px));

     theta1 = (Gradient_X*Gradient_PX + Gradient_Y*Gradient_PY + 1) / ((sqrt(Gradient_Y*Gradient_Y + Gradient_X*Gradient_X + 1))*(sqrt(Gradient_PY*Gradient_PY + Gradient_PX*Gradient_PX + 1)));*/

    signed short Gradient_X, Gradient_Y;
    Gradient_X = 0.5*(depthFrame.at<int>(y, x + 1) - depthFrame.at<int>(y, x - 1));
    Gradient_Y = 0.5*(depthFrame.at<int>(y + 1, x) - depthFrame.at<int>(y - 1, x));

    if (Gradient_X != 0)
        theta1 = atan(Gradient_Y / Gradient_X) / pi * 180;
    else if (Gradient_Y > 0)
        theta1 = 90;
    else
        theta1 = -90;
    theta2 = atan(sqrt(Gradient_Y*Gradient_Y + Gradient_X*Gradient_X)) / pi * 180;

    return 0;
}