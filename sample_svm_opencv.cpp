#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace ml;

int main()
{
	int width = 512;
	int height = 512;

	Mat image = Mat::zeros(height, width, CV_8UC3);

	//建立训练样本
	int labels[10] = { 1, -1, 1, 1, -1, 1, -1, 1, -1, -1 };
	float trainingData[10][2] = { {501,150},{255,10},{501,255},{10,501},{25,80},{150,300},{77,200},{300,300},{45,250},{200,200} };
	Mat labelsMat(10, 1, CV_32SC1, labels);
	Mat trainingDataMat(10, 2, CV_32FC1, trainingData);

	Ptr<SVM> svm = SVM::create();//创建分类器
	svm->setType(SVM::C_SVC);//SVM类型
	svm->setKernel(SVM::LINEAR);//核函数的类型
	svm->setTermCriteria(TermCriteria(TermCriteria::EPS, 1000, FLT_EPSILON));//算法终止条件

	//设置训练数据并训练分类器
	Ptr<TrainData> tData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	svm->train(tData);

	Vec3b green(0, 255, 0), blue(255, 0, 0);
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			Mat sampleMat = (Mat_<float>(1, 2) << j, i);
			float response = svm->predict(sampleMat);
			//float response = SVM.predict(sampleMat); 这里有不同

			if (response == 1)
			{
				image.at<Vec3b>(i, j) = green;
			}
			else if (response == -1)
			{
				image.at<Vec3b>(i, j) = blue;
			}
		}
	}

	//显示训练数据
	int thickness = -1;
	int lineType = 8;
	Scalar c1 = Scalar::all(0);   //标记为1的显示成黑点
	Scalar c2 = Scalar::all(255); //标记成-1的显示成白点
	for (int i = 0; i < labelsMat.rows; i++)
	{
		const float* v = trainingDataMat.ptr<float>(i);
		Point pt = Point((int)v[0], (int)v[1]);
		if (labels[i] == 1)
		{
			circle(image, pt, 5, c1, thickness, lineType);
		}
		else
		{
			circle(image, pt, 5, c2, thickness, lineType);
		}
	}
	imshow("SVM", image);
	waitKey(0);
}
