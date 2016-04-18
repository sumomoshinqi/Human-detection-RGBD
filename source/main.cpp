#include "HeadDetect.h"

//int main(int argc, _TCHAR* argv[])
//{
//    HeadDetect hd;
//    hd.Run();
//    cout << "======Process Ended======" << endl;
//    /*char ch[50] = "-1,5,10,-0.868,1.52,-5";
//     for (int i = 0; i < strlen(ch); i++)
//     cout << ch[i] << endl;*/
//    //double a = -0.8888;
//    //cout << a << endl;
//    //getchar();
//    return 0;
//}

//void buildRandomForestClassifier(const char* _dataFileName);
//
void main(void)
{
	printf("\nThis is dataset train set.\n"
		"Classifier is Random Trees\n");
	printf("Please wait...\n");

	const char* dataFileName = "/home/sumomoshinqi/Develop/CV/Human-detection-RGBD/source/47_label/cloth_38_47_normal.data";
	buildRandomForestClassifier(dataFileName);
}

void buildRandomForestClassifier(const char* _dataFileName)
{
	// 读取数据
	const int MAX_FEATURE = 100;// 2;// 100;
	const int VAR_COUNT = 40;// 40;// 2;// 20;
	const int NSAMPLES_ALL = 5979;// 5;// 4300;

	FILE* f = fopen(_dataFileName, "rt");//rt代表以文本模式打开文件

	char buf[MAX_FEATURE + 2];
	float* el_ptr;

	Mat data(NSAMPLES_ALL, 40, CV_32F);
	Mat responses(NSAMPLES_ALL, 1, CV_32F);

	el_ptr = new float[VAR_COUNT + 1];

	float a = 0, b = 0;

	for (int i = 0; i<NSAMPLES_ALL; i++)
	{
		fscanf(f, "%f", &a);
		responses.at<float>(i, 0) = a;
		for (int p = 0; p < 40; p++)         //注意！！！这里也要改成20或40或60！！！
		{
			fscanf(f, ",%f", &b);
			data.at<float>(i, p) = b;
		}
		fscanf(f, "\n");
	}
	fclose(f);
	delete el_ptr;

	// 训练随机森林
	int ntrain_samples = 5979;//50;//1155;//(int)(NSAMPLES_ALL * 0.8);//16000 作为训练
	int i = 0;
	CvRTrees forest;
	/*CvRTrees就是随机森林的主体了。包涵了train训练函数、predict预测函数、predict_prob返回预测分类标签、
	getVarImportance返回变量权重矩阵、get_proximity返回两训练样本之间的相似度、calc_error返回随机森林
	的预测误差、get_train_error返回训练误差、get_rng返回使用随机数的当前值、get_tree_count返回构造随
	机森林的树的数目、get_tree返回构造随机森林的其中一棵树。*/

	forest.train(data, CV_ROW_SAMPLE, responses, Mat(), Mat(), Mat(), Mat(),                          //这里用到的是data来作为训练，因此用到的是全部数据（NSAMPLES_ALL个数据）
		CvRTParams(10, 10, 0, false, 15, 0, true, 4, 100, 0.01f, CV_TERMCRIT_ITER));

	forest.save("/home/sumomoshinqi/Develop/CV/Human-detection-RGBD/source/47_label/cloth_38_47_normal.xml");
	cout << "finish!\n" << endl;
	system("pause");
}
