// test the operator (reshape, create) of the Mat data structure of OpenCV
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat img1(10,10,CV_8UC1,50);
	cout << (int)img1.at<uchar>(1, 1) << endl;
	img1.reshape(5, 20);
	//img1.create(5,20,CV_8UC1);
	cout << (int)img1.at<uchar>(1, 1) << endl;
	img1.create(6, 20, CV_8UC1);
	cout << img1.at<uchar>(1, 1) << endl;
	
	//
	char c;
	cin >> c;
	return 0;
}
