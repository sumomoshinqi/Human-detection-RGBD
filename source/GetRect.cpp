//
//#include "GetRect.h"
//
////COpenNI g_openNi;
////和hod冲突
//GetRect::GetRect(){
//	Pos = cv::Mat(40, 7, CV_32FC1);//cv::Mat(25,6,CV_32FC1);
//	Ign = cv::Mat(40, 5, CV_32FC1);
//
//	MatHelper::ReadMat("32F", Pos, "Pos.txt");
//	MatHelper::ReadMat("32F", Ign, "Ign.txt");
//	//printf("mat read ok\n");
//
//	/////////////////////////////////////
//	//     printf("统计样本个数;\n");
//	//     TotalNum = 1485;
//	//     PosNum = 0;
//	//     IgnNum = 0;
//	//
//	//     printf("TotalNum = %d\n",TotalNum);
//	//
//	//     for(int Posj = 0; Posj < Pos.rows; Posj++ )
//	//         for(int Posi = 0; Posi < Pos.cols; Posi++ ){
//	//             if( Pos.at<float>(Posj,Posi) != -1)
//	//                 PosNum = PosNum + 1 ;
//	//         }
//	//     printf("PosNum = %d\n",PosNum);
//	//
//	//     for(int Ignj = 0; Ignj < Ign.rows; Ignj++ )
//	//         for(int Igni = 0; Igni < Ign.cols; Igni++ ){
//	//             if ( Ign.at<float>(Ignj,Igni) != -1)
//	//                 IgnNum = IgnNum + 1 ;
//	//         }
//	//     printf("IgnNum = %d\n",IgnNum);
//	//
//	//     printf("负样本个数; = %d",TotalNum - PosNum -IgnNum );
//	//
//	//     getchar();
//	//
//	cvsvm.load("SVM_HOG.xml");//从XML文件读取训练好的SVM模型
//
//}
//
//GetRect::~GetRect(){
//
//}
//
//int PosRow = -1, IgnRow = -1, PosCol, IgnCol, sampleFeatureMatRow = 0, sampleFeatureMatCol;
//cv::Mat sampleFeatureMat = cv::Mat::zeros(PosSamNO + NegSamNO, DescriptorDim, CV_32FC1);
////所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数 ;
//cv::Mat sampleLabelMat = cv::Mat::zeros(PosSamNO + NegSamNO, 1, CV_32FC1);
////初始化训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，0表示无人;
//
//int cKey;
//int label0 = 0, label1 = 0;
////int TotallllllNum = 0;
//
////void GetRect::getrectimg(cv::Mat& m_oriBgrResultImage_8UC3, cv::Mat& m_preproResultImage_16UC1)
////{
////	int t = 0, s = 0, k = 0, li = 0, lj = 0, r, r2, pointnum = 0;
////	char buffer[200];
////	cv::Mat m_oriBgrResultImage2_8UC3, m_oriBgrResultImage3_8UC3;//copyto;
////	m_oriBgrResultImage_8UC3.copyTo(m_oriBgrResultImage2_8UC3);
////	m_oriBgrResultImage_8UC3.copyTo(m_oriBgrResultImage3_8UC3);
////	cv::Mat m_preproResultImage2_16UC1;
////	m_preproResultImage_16UC1.copyTo(m_preproResultImage2_16UC1);
////
////	int subarow;
////
////	cv::Mat subc = cv::Mat(3, 1, CV_32FC1);
////
////	//learning
////
////	//HOG
////	cv::Mat RectRGBImg;
////	cv::Mat resizedRGBImage;
////	cv::Mat GrayRGBImg = cv::Mat(66, 50, CV_32FC1);
////	cv::HOGDescriptor hog(cvSize(48, 64), cvSize(32, 32), cvSize(16, 16), cvSize(16, 16), 8);
////
////	// Normal Vectors
////	cv::Mat RectDImg;
////	cv::Mat resizedDImage;
////	vector<float>abc;//feature[A/C,B/C]
////	float b[2];//插入到每个abc里面的值[A/C,B/C];
////
////	//深度差;
////	cv::Mat DepthDif;
////	cv::Mat resizedDepthDif;
////	//FILE *fDetectee, *fOverlapper, *fBackground;
////
////	//     cout << "Pos = " << endl << " " << Pos << endl << endl;
////
////	if (
////		g_openNi.m_ImageGenerator.GetFrameID() % 50 == 0
////		&&
////		g_openNi.m_ImageGenerator.GetFrameID() <= 2000
////		)
////	{
////		//learning
////		PosRow++;
////		PosCol = 0;
////		IgnRow++;
////		IgnCol = 0;
////		sampleFeatureMatCol = 0;
////
////
////		for (int j = 50; j < m_preproResultImage2_16UC1.rows; ++j)
////		{
////			cv::circle(m_oriBgrResultImage_8UC3, cv::Point(540, j), 1, cv::Scalar(255, 255, 255), -1, 8); //白色;
////
////			for (int i = 1; i < m_preproResultImage2_16UC1.cols; ++i)//540
////			{
////				if (m_preproResultImage2_16UC1.at<unsigned short>(j, i) > 800 && m_preproResultImage2_16UC1.at<unsigned short>(j, i) < 6000)
////					// 此点的深度值在允许范围内;
////				{
////					if (	 //两点深度差允许在一定范围内;
////						abs(m_preproResultImage2_16UC1.at<unsigned short>(j, i - 1) - m_preproResultImage2_16UC1.at<unsigned short>(j, i)) > 200)
////					{
////						t = 0;
////						r = Data3DHelper::GetSizeInImageBySizeIn3D(150, m_preproResultImage2_16UC1.at<unsigned short>(j, i));
////						r2 = Data3DHelper::GetSizeInImageBySizeIn3D(120, m_preproResultImage2_16UC1.at<unsigned short>(j, i));
////						// 现实中的宽度在相对的深度中所现实的宽度;
////						//取上一行的点;
////						for (k = r; k>-r && i + k >0 && i + k < 640; k--){
////							if (abs(m_preproResultImage2_16UC1.at<unsigned short>(j, i) - m_preproResultImage2_16UC1.at<unsigned short>(j - 1, i + k)) < 180)
////								//abs是因为背景可能为0
////								t = t + 1;//不是顶点;
////							else
////								t = t + 0;	//是顶点;
////						}//for
////						if (t == 0){
////							s = 0;
////							for (lj = -2; lj < 1; lj++){
////								for (li = -1; li > -r && i + li > 0 && i + li< 640; li--){
////									if (
////										m_oriBgrResultImage3_8UC3.at<cv::Vec3b>(j + lj, i + li)[0] == 0 &&
////										m_oriBgrResultImage3_8UC3.at<cv::Vec3b>(j + lj, i + li)[1] == 255 &&
////										m_oriBgrResultImage3_8UC3.at<cv::Vec3b>(j + lj, i + li)[2] == 255     //黄色;
////										)
////										s = s + 1;//之前已经标记过点了;
////									else
////										s = s + 0;	//之前没有标记过的点;
////								}//for li
////							}//for lj
////							if (s == 0){
////								//pointnum++;
////								// printf("frame = %d\n",g_openNi.m_ImageGenerator.GetFrameID());
////								// printf("pointnum = %d\n",pointnum);
////								// printf("j = %d\t i = %d\n",j,i);
////								//  printf("r = %d\n",r);
////								cv::circle(m_oriBgrResultImage3_8UC3, cv::Point(i, j), 3, cv::Scalar(0, 255, 255), -1, 8);//黄色;
////
////								if (int(i - 1.5*r - 1) < 0 || int(i + 1.5*r + 1) > 640 || int(j + 3.3*r + 1) > 480 || int(j - 0.7*r - 1) < 0)
////								{
////									//框取不到;
////									// printf("取不到;\n");
////									continue;
////								}
////								else
////								{//框取的到的情况;
////									pointnum++;
////
////									if (TRAIN)//若TRAIN为true，重新训练分类器 ;
////									{
////										//printf("pointnum = %d\n",pointnum);
////										if (pointnum == Ign.at<float>(IgnRow, IgnCol)){
////											//不考虑;
////											IgnCol++;
////											// printf("忽略;\n");
////											// printf("IgnRow, IgnCol = %d\t%d\n",IgnRow,IgnCol);
////											continue;
////										}// if 框取不到;
////										else{//除去框取不到的点;
////											// printf("r= %d\n",r);
////
////											// //                                                 ========================================================
////											//                                                 //Copy Mat  HOG
////											// 										        RectRGBImg.create(4*r+2,3*r+2,CV_8UC3);//rows, cols, type
////											// 										        MatHelper::GetRectMat(m_oriBgrResultImage2_8UC3,RectRGBImg,int(i-1.5*r-1),int(j-0.7*r-1),3*r+2,4*r+2);//width height
////											//                                                 cv::resize(RectRGBImg,resizedRGBImage,cv::Size(48,64));//col row
////											//                                                 RectRGBImg.release();
////											//                                                 vector<float>feature;//结果数组;
////											//                                                 hog.compute(resizedRGBImage, feature,cv::Size(16,16),cv::Size(0,0)); //调用计算函数开始计算;
////											//                                                 //Copy Mat  HOG
////											//                                                 //========================================================
////
////
////
////											//=======================================================
////											//Normal Vector
////											//  printf("Normal Vector\n");
////
////											RectDImg.create(4 * r, 3 * r, CV_16UC1);
////											MatHelper::GetRectDepthMat(m_preproResultImage2_16UC1, RectDImg, int(i - 1.5*r), int(j - 0.7*r), 3 * r, 4 * r);
////											resizedDImage.create(48, 64, CV_16UC1);
////											// MatHelper::ResizedDepthMat(RectDImg,resizedDImage,3*r,4*r,48,64);
////											cv::resize(RectDImg, resizedDImage, cv::Size(48, 64));
////
////											abc.clear();
////											for (int resizedDImageRow = 0; resizedDImageRow < resizedDImage.rows; resizedDImageRow = resizedDImageRow + 4){
////												for (int resizedDImageCol = 0; resizedDImageCol < resizedDImage.cols; resizedDImageCol = resizedDImageCol + 4){
////
////													subarow = 0;
////													for (int subresizerow = 0; subresizerow < 4; subresizerow++){
////														for (int subresizecol = 0; subresizecol < 4; subresizecol++){
////
////															if (resizedDImage.at<unsigned short>
////																(resizedDImageRow + subresizerow,
////																resizedDImageCol + subresizecol) != 0)
////																subarow++;
////														}
////													}
////
////													if (subarow >= 3){
////														cv::Mat suba = cv::Mat(subarow, 3, CV_32FC1);
////														int subarowi = 0;
////														for (int subresizerow = 0; subresizerow < 4; subresizerow++){
////															for (int subresizecol = 0; subresizecol < 4; subresizecol++){
////
////																if (resizedDImage.at<unsigned short>
////																	(resizedDImageRow + subresizerow,
////																	resizedDImageCol + subresizecol) != 0){
////
////																	XnPoint3D pos = Data3DHelper::Get3DCoordinate(
////																		resizedDImageRow + subresizerow,
////																		resizedDImageCol + subresizecol,
////																		resizedDImage.at<unsigned short>
////																		(resizedDImageRow + subresizerow,
////																		resizedDImageCol + subresizecol));
////
////																	suba.at<float>(subarowi, 0) = pos.X;
////																	suba.at<float>(subarowi, 1) = pos.Y;
////																	suba.at<float>(subarowi, 2) = pos.Z;
////
////																	subarowi++;
////																}
////															}//for subresizecol 4
////														}//for subresizerow 4
////
////														int subbrow = subarow;
////														cv::Mat subb = cv::Mat(subbrow, 1, CV_32FC1);
////
////														for (int subbrowi = 0; subbrowi < subbrow; subbrowi++){
////															subb.at<float>(subbrowi, 0) = 1.0;
////														}
////														cv::solve(suba, subb, subc, CV_SVD);
////
////														abc.push_back(subc.at<float>(0, 0) / subc.at<float>(2, 0));
////														abc.push_back(subc.at<float>(1, 0) / subc.at<float>(2, 0));
////														//getchar();
////
////													}// if subarow >= 3
////													else{// subarow < 3
////														// 深度=0的点很多,条件太少无法拟合平面;
////
////														abc.push_back(0);
////														abc.push_back(0);
////													}
////
////												}//for col
////											}//for row
////
////											RectDImg.release();
////											resizedDImage.release();
////
////											//                                                 for(std::vector<float>::iterator m = abc.begin(); m != abc.end(); m++ )    //用迭代器的方式输出容器对象的值;
////											//                                                 {
////											//                                                     cout<<*m<<endl;
////											//                                                 }
////											//                                                 getchar();
////
////											vector<float>feature;//feature[A/C,B/C]
////
////											feature = abc;
////											//                                                 printf("feature.size = %d\n",feature.size());
////											//                                                 getchar();
////											//                                                 for(std::vector<float>::iterator m = feature.begin(); m != feature.end(); m++ )    //用迭代器的方式输出容器对象的值;
////											//                                                 {
////											//                                                     cout<<*m<<endl;
////											//                                                 }
////											//                                                 getchar();
////											//Normal Vector
////											//=======================================================
////
////
////											//判断正or负 读文件;
////											if (pointnum == Pos.at<float>(PosRow, PosCol)){
////												//正样本 ;
////												for (sampleFeatureMatCol = 0; sampleFeatureMatCol < DescriptorDim; sampleFeatureMatCol++)
////													sampleFeatureMat.at<float>(sampleFeatureMatRow, sampleFeatureMatCol) = feature[sampleFeatureMatCol];
////
////												sampleLabelMat.at<float>(sampleFeatureMatRow, 0) = 1;
////												sampleFeatureMatRow++;
////												PosCol++;
////
////												//                                                     //=======================================================
////												//                                                     // 取深度差; detectee overlapper background
////												//
////												//                                                     if( m_preproResultImage2_16UC1.at<unsigned short>(j,i) != 0 ){
////												//
////												//                                                         cv::rectangle(m_oriBgrResultImage_8UC3,cv::Rect(i-r2,j-1,2*r2,2*r2),cv::Scalar(0,255,255),1,8);//黄色框
////												//
////												//                                                         DepthDif.create(2*r2, 2*r2, CV_16UC1);
////												//                                                         MatHelper::GetRectDepthMat(m_preproResultImage2_16UC1, DepthDif,int(i-r2),int(j-1),2*r2,2*r2 );
////												//
////												//                                                         resizedDepthDif.create(24, 24, CV_16UC1);
////												//                                                         MatHelper::ResizedDepthMat(DepthDif,resizedDepthDif,2*r2,2*r2,24,24);
////												//
////												//                                                         for ( int resizedDepthDifrow = 0; resizedDepthDifrow < resizedDepthDif.rows; resizedDepthDifrow++)
////												//                                                         {
////												//                                                             for ( int resizedDepthDifcol = 0; resizedDepthDifcol < resizedDepthDif.cols; resizedDepthDifcol++)
////												//                                                             {
////												//                                                                 if( resizedDepthDif.at<unsigned short>(resizedDepthDifrow,resizedDepthDifcol) != 0)
////												//                                                                 {
////												//
////												//                                                                     printf("resizedDepthDif\t%d\n",resizedDepthDif.at<unsigned short>(resizedDepthDifrow,resizedDepthDifcol));
////												//                                                                     printf("m_preproResultImage_16UC1 j i \t%d\n",m_preproResultImage_16UC1.at<unsigned short>(j,i));
////												//
////												//                                                                     getchar();
////												//                                                                  signed short DepthDifference =
////												//                                                                      resizedDepthDif.at<unsigned short>(resizedDepthDifrow,resizedDepthDifcol)
////												//                                                                      - m_preproResultImage2_16UC1.at<unsigned short>(j,i) ;
////												//
////												//                                                                  printf("DepthDifferent\t%hd\n",DepthDifference);
////												//                                                                  //getchar();
////												//
////												//                                                                  if ( abs (DepthDifference) <= 120 )// detectee;
////												//                                                                  {
////												//                                                                      //fDetectee = fopen( "detectee.txt","w");
////												// //                                                                      printf("detectee;\t%u\n",intDepthDifferent);
////												// //                                                                      getchar();
////												//                                                                      ofstream  fDetectee("detectee.txt", /*ios::out ||*/ ios::app);//??
////												//                                                                      fDetectee << DepthDifference << endl;
////												//                                                                      fDetectee.close();
////												//                                                                  }//detectee;
////												//                                                                  else
////												//                                                                  {
////												//                                                                      if ( DepthDifference < -120) //overlapper;
////												//                                                                      {
////												// //                                                                          printf("overlapper;\t%u\n",intDepthDifferent);
////												// //                                                                          getchar();
////												//                                                                          ofstream  fOverlapper("overlapper.txt", /*ios::out ||*/ ios::app);//??
////												//                                                                          fOverlapper << DepthDifference << endl;
////												//                                                                          fOverlapper.close();
////												//                                                                      }//overlapper;
////												//                                                                      else
////												//                                                                      {
////												//                                                                          if( DepthDifference > 120 )// background;
////												//                                                                          {
////												// //                                                                              printf("background\t%u\n",intDepthDifferent);
////												// //                                                                              getchar();
////												//                                                                              ofstream  fBackground("background.txt", /*ios::out ||*/ ios::app);//??
////												//                                                                              fBackground << DepthDifference << endl;
////												//                                                                              fBackground.close();
////												//                                                                          }//background;
////												//                                                                      }
////												//                                                                  }// overlapper or background;
////												//                                                                 }// if depth ( y,x) != 0
////												//                                                             }//for col;
////												//                                                         }//for row;
////												//
////												//                                                         DepthDif.release();
////												//                                                         resizedDepthDif.release();
////												//                                                     }//if depth (j,i) != 0
////												//
////												//                                                     //取深度差; detectee overlapper background
////												//                                                     //===============================
////
////												cv::rectangle(m_oriBgrResultImage_8UC3, cv::Rect(i - 1.5*r, j - 0.7*r, 3 * r, 4 * r), cv::Scalar(0, 0, 255), 1, 8); //红色 //小框;
////												char buf1[200];
////												sprintf(buf1, "%d", pointnum);
////												cv::putText(m_oriBgrResultImage_8UC3, buf1, cvPoint(i, j), CV_FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(0, 255, 0)); //绿色;
////
////												cv::circle(m_preproResultImage_16UC1, cv::Point(i, j), 3, cv::Scalar(0, 255, 255), -1, 8);
////												cv::rectangle(m_preproResultImage_16UC1, cv::Rect(i - 1.5*r, j - 0.7*r, 3 * r, 4 * r), cv::Scalar(0, 0, 255), 1, 8);
////												//cv::rectangle(m_preproResultImage_16UC1,cv::Rect(i-r2,j-1,2*r2,2*r2),cv::Scalar(0,0,255),1,8);
////											}
////											else{
////												//负样本;
////												for (sampleFeatureMatCol = 0; sampleFeatureMatCol < DescriptorDim; sampleFeatureMatCol++)
////													sampleFeatureMat.at<float>(sampleFeatureMatRow, sampleFeatureMatCol) = feature[sampleFeatureMatCol];
////
////												sampleLabelMat.at<float>(sampleFeatureMatRow, 0) = -1;
////												sampleFeatureMatRow++;
////
////												//                                                         cv::rectangle(m_oriBgrResultImage_8UC3,cv::Rect(i-1.5*r,j-0.7*r,3*r,4*r),cv::Scalar( 255,0,0),1,8); //蓝色 //小框;
////												//                                                         char buf1[200];
////												//                                                         sprintf(buf1,"%d",pointnum);
////												//                                                         cv::putText(m_oriBgrResultImage_8UC3,buf1,cvPoint(i,j),CV_FONT_HERSHEY_DUPLEX,0.5,CV_RGB(0,255,0)); //绿色;
////											}
////										}//else 除去框取不到的点;
////									}// TRAIN = true
////
////
////									else
////										//若TRAIN为false，从XML文件读取训练好的分类器;
////									{
////										//                                         //========================================================
////										//                                         //Copy Mat  HOG
////										//                                         RectRGBImg.create(4*r+2,3*r+2,CV_8UC3);//rows, cols, type
////										//                                         MatHelper::GetRectMat(m_oriBgrResultImage2_8UC3,RectRGBImg,int(i-1.5*r-1),int(j-0.7*r-1),3*r+2,4*r+2);//width height
////										//                                         cv::resize(RectRGBImg,resizedRGBImage,cv::Size(48,64));//col row
////										//                                         RectRGBImg.release();
////										//                                         vector<float>feature;//结果数组;
////										//                                         hog.compute(resizedRGBImage, feature,cv::Size(16,16)); //调用计算函数开始计算;
////										//                                         //Copy Mat  HOG
////										//                                         //========================================================
////
////										//=======================================================
////										//Normal Vector
////										//  printf("Normal Vector\n");
////
////										RectDImg.create(4 * r, 3 * r, CV_16UC1);
////										MatHelper::GetRectDepthMat(m_preproResultImage2_16UC1, RectDImg, int(i - 1.5*r), int(j - 0.7*r), 3 * r, 4 * r);
////										resizedDImage.create(48, 64, CV_16UC1);
////										cv::resize(RectDImg, resizedDImage, cv::Size(48, 64));
////										abc.clear();
////										for (int resizedDImageRow = 0; resizedDImageRow < resizedDImage.rows; resizedDImageRow = resizedDImageRow + 4){
////											for (int resizedDImageCol = 0; resizedDImageCol < resizedDImage.cols; resizedDImageCol = resizedDImageCol + 4){
////												subarow = 0;
////												for (int subresizerow = 0; subresizerow < 4; subresizerow++){
////													for (int subresizecol = 0; subresizecol < 4; subresizecol++){
////														if (resizedDImage.at<unsigned short>
////															(resizedDImageRow + subresizerow,
////															resizedDImageCol + subresizecol) != 0)
////															subarow++;
////													}
////												}
////												if (subarow >= 3){
////													cv::Mat suba = cv::Mat(subarow, 3, CV_32FC1);
////													int subarowi = 0;
////													for (int subresizerow = 0; subresizerow < 4; subresizerow++){
////														for (int subresizecol = 0; subresizecol < 4; subresizecol++){
////															if (resizedDImage.at<unsigned short>
////																(resizedDImageRow + subresizerow,
////																resizedDImageCol + subresizecol) != 0){
////																XnPoint3D pos = Data3DHelper::Get3DCoordinate(
////																	resizedDImageRow + subresizerow,
////																	resizedDImageCol + subresizecol,
////																	resizedDImage.at<unsigned short>
////																	(resizedDImageRow + subresizerow,
////																	resizedDImageCol + subresizecol));
////																suba.at<float>(subarowi, 0) = pos.X;
////																suba.at<float>(subarowi, 1) = pos.Y;
////																suba.at<float>(subarowi, 2) = pos.Z;
////																subarowi++;
////															}
////														}//for subresizecol 4
////													}//for subresizerow 4
////													int subbrow = subarow;
////													cv::Mat subb = cv::Mat(subbrow, 1, CV_32FC1);
////													for (int subbrowi = 0; subbrowi < subbrow; subbrowi++){
////														subb.at<float>(subbrowi, 0) = 1.0;
////													}
////													cv::solve(suba, subb, subc, CV_SVD);
////													abc.push_back(subc.at<float>(0, 0) / subc.at<float>(2, 0));
////													abc.push_back(subc.at<float>(1, 0) / subc.at<float>(2, 0));
////												}// if subarow >= 3
////												else{// 深度=0的点很多,条件太少无法拟合平面;
////													abc.push_back(0);
////													abc.push_back(0);
////												}
////											}//for col
////										}//for row
////										RectDImg.release();
////										resizedDImage.release();
////										vector<float>feature;//feature[A/C,B/C]
////										feature = abc;
////										//Normal Vector
////										//======================================
////
////
////
////										cv::Mat ResponseMat = cv::Mat(1, DescriptorDim, CV_32FC1);
////										for (int responsecol = 0; responsecol < DescriptorDim; responsecol++)
////											ResponseMat.at<float>(0, responsecol) = feature[responsecol];
////										float response = cvsvm.predict(ResponseMat);//, true);
////										// printf("response = %f\n",response);
////										int response1 = int(response);
////										// printf(" response1 = %d\n ",response1);
////										// getchar();
////
////										if (response1 == -1){// > -2.5 ??
////											//负样本;
////											//  printf("负样本;\n");
////											// cv::rectangle(m_oriBgrResultImage_8UC3,cv::Rect(i-1.5*r,j-0.7*r,3*r,4*r),cv::Scalar(255,0,0),1,8); //蓝色 //小框;
////											continue;
////										}
////										else {
////											//正样本;
////											// printf("正样本;\n");
////											cv::circle(m_oriBgrResultImage_8UC3, cv::Point(i, j), 3, cv::Scalar(0, 255, 255), -1, 8);//黄色;
////											cv::rectangle(m_oriBgrResultImage_8UC3, cv::Rect(i - 1.5*r, j - 0.7*r, 3 * r, 4 * r), cv::Scalar(0, 0, 255), 1, 8); //红色 //小框;
////											char buf1[200];
////											sprintf(buf1, "%d", pointnum);
////											cv::putText(m_oriBgrResultImage_8UC3, buf1, cvPoint(i, j), CV_FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(0, 255, 0)); //绿色;
////
////											//深度图像;
////											cv::circle(m_preproResultImage_16UC1, cv::Point(i, j), 3, cv::Scalar(0, 255, 255), -1, 8);
////											cv::rectangle(m_preproResultImage_16UC1, cv::Rect(i - 1.5*r, j - 0.7*r, 3 * r, 4 * r), cv::Scalar(0, 0, 255), 1, 8);
////										}
////										// }//框取的到的情况;
////									}//TRAIN =false
////
////									//不训练的时候;
////
////
////
////
////									//                                       cv::rectangle(m_oriBgrResultImage_8UC3,cv::Rect(i-1.5*r,j-0.7*r,3*r,4*r),cv::Scalar(0,0,255),1,8); //红色 //小框;
////									//                                       char buf1[200];
////									//                                       sprintf(buf1,"%d",pointnum);
////									//                                       cv::putText(m_oriBgrResultImage_8UC3,buf1,cvPoint(i,j),CV_FONT_HERSHEY_DUPLEX,0.5,CV_RGB(0,255,0)); //绿色;
////
////								}//框取的到;
////							}//if s = 0
////						}//if t=0
////					}//if 临近点深度差;
////				} //if 此点的深度值在允许范围内 ;
////			}//for i
////		}//for j
////
////
////
////	}//frame % 50 && <= 1250
////
////
////	else
////	{
////		if (g_openNi.m_ImageGenerator.GetFrameID() % 50 == 0
////			&& g_openNi.m_ImageGenerator.GetFrameID() <= 2050
////			&&
////			g_openNi.m_ImageGenerator.GetFrameID()> 2000)
////		{
////			//1250帧跑完
////
////			//cout << "sampleFeatureMat = " << endl << " " << sampleFeatureMat << endl << endl;
////			//getchar();
////			printf("sampleFeatureMat rows clos; = %d\n%d\n", sampleFeatureMat.rows, sampleFeatureMat.cols);
////			// getchar();
////			//cout << "sampleLabelMat = " << endl << " " << sampleLabelMat << endl << endl;
////			printf("sampleLabelMat rows clos; = %d\n%d\n", sampleLabelMat.rows, sampleLabelMat.cols);
////			//
////			// getchar();
////			//训练SVM分类器  ;
////			MatHelper::PrintMat("32F", sampleFeatureMat, "sampleFeatureMat.txt");
////			MatHelper::PrintMat("32F", sampleLabelMat, "sampleLabelMat.txt");
////			printf("print ok!\n");
////
////			CvSVMParams param;
////			param.svm_type = CvSVM::C_SVC;
////			//param.C           = 0.1;
////			param.kernel_type = CvSVM::LINEAR;
////			param.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 3000, 1e-7);// 100
////
////			//迭代终止条件，当迭代满1000次或误差小于FLT_EPSILON时停止迭代
////			//                   CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
////			//                   //SVM参数：SVM类型为C_SVC；线性核函数；松弛因子C=0.01  ;
////			//                 //  CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);
////			//                   CvSVMParams param(CvSVM::C_SVC, CvSVM::RBF, 0, 1, 0, 1, 0, 0, 0, criteria);
////			// Set up SVM's parameters
////
////			cout << "开始训练SVM分类器" << endl;
////			//CvSVM cvsvm;
////			cvsvm.train(sampleFeatureMat, sampleLabelMat, cv::Mat(), cv::Mat(), param);//训练分类器  ;
////			cout << "训练完成" << endl;
////			int supportVectorNum = cvsvm.get_support_vector_count();//支持向量的个数 ;
////			cout << "支持向量个数：" << supportVectorNum << endl;
////
////			for (int tempFeatureMat = 0; tempFeatureMat < PosSamNO + NegSamNO; tempFeatureMat++){
////				cv::Mat ResponseMat = cv::Mat(1, DescriptorDim, CV_32FC1);
////				for (int responsecol = 0; responsecol < DescriptorDim; responsecol++)
////					ResponseMat.at<float>(0, responsecol) = sampleFeatureMat.at<float>(tempFeatureMat, responsecol);
////				float response = cvsvm.predict(ResponseMat);
////				int response1 = int(response);
////				// printf("label = %d\n",int(sampleLabelMat.at<float>(temp,0)));
////				// printf("response1 = %d\n",response1);
////				if (int(sampleLabelMat.at<float>(tempFeatureMat, 0)) == 1 && response1 == -1){
////					//  printf(" 1 -- -1 :%d\n",tempFeatureMat);
////					label1++;
////				}
////				if (int(sampleLabelMat.at<float>(tempFeatureMat, 0)) == -1 && response1 == 1){
////					// printf(" -1 -- 1 :%d\n",tempFeatureMat);
////					label0++;
////				}
////
////			}
////			printf("label = 1 response1 = -1 : %d\n", label1);
////			printf("label = -1 response1 = 1 : %d\n", label0);
////			cvsvm.save("SVM_HOG.xml");//将训练好的SVM模型保存为xml文件;
////
////			getchar();
////
////		}// frame 1250 - 1300
////	}//else
////
////}
//
//
