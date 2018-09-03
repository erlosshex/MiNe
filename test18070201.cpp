#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
	double time0 = static_cast<double>(cv::getTickCount());
	cv::Mat img = cv::imread("F:/Codes/Mine/TestImg/1.bmp");
	//img(cv::Range(100, 150),cv::Range(150, 300)) = cv::Scalar(255,255,255);
	img(cv::Rect(150,100,150,50)) = cv::Scalar(255, 255, 255);

	cout << "cols(x) : " << img.cols << endl;
	cout << "rows(y) : " << img.rows << endl;
	cout << "type : " << img.type() << endl;

	//imgROI = cv::Mat(imgROI.rows, imgROI.cols, img.type(), cv::Scalar(255, 255, 255));

	cv::line(img, cv::Point(100, 100), cv::Point(100, 250), cv::Scalar(255,255,255), 10);

	cv::namedWindow("picture", cv::WINDOW_NORMAL);
	cv::imshow("picture", img);
	double time1 = static_cast<double>(cv::getTickCount());
	cout << "time : " << (time1 - time0) / cv::getTickFrequency()<<endl;
	cv::waitKey();

	return 0;
}
