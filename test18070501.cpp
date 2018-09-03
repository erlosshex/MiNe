#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat m1(100, 90, CV_8UC1, Scalar(100));
	Mat m2(100, 90, CV_8UC3, Scalar(1, 2, 3));
	Mat m3 = imread("E:/培训资料整理/20170803（课程用）/PGA.BMP");
	cout << "m1 depth : "<< m1.channels() << endl;
	cout << "m2 depth : "<< m2.channels() << endl;
	cout << "m3 depth : "<< m3.channels() << endl;
	cout << "element 0 : " << int(m3.at<Vec3b>(1, 2)[0]) << endl;
	cout << "element 1 : " << int(m3.at<Vec3b>(1, 2)[1]) << endl;
	cout << "element 2 : " << int(m3.at<Vec3b>(1, 2)[2]) << endl;

	uchar *p = m3.ptr<uchar>(1);
	cout << "ptr 0 : " << int(*(p + 0)) << endl;
	cout << "ptr 1 : " << int(*(p + 1)) << endl;
	cout << "ptr 2 : " << int(*(p + 2)) << endl;
	char c;
	cin >> c;
	return 0;
}
