#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat img(3, 3, CV_32FC3,Scalar(32,45,61));
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
			cout << img.at<Point3f>(row, col) << "  ";
		cout << endl;
	}
	cout << "********************************************" << endl;
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
			cout << img.at<Vec3f>(row, col) << "  ";
		cout << endl;
	}
	cout << "********************************************" << endl;
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
			cout << img.at<Vec3f>(row, col)[0] << "  ";
		cout << endl;
	}
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
			cout << img.at<Vec3f>(row, col)[1] << "  ";
		cout << endl;
	}
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
			cout << img.at<Vec3f>(row, col)[2] << "  ";
		cout << endl;
	}
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
			cout << img.at<Point3f>(row, col).x << "  ";
		cout << endl;
	}
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
			cout << img.at<Point3f>(row, col).y << "  ";
		cout << endl;
	}
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
			cout << img.at<Point3f>(row, col).z << "  ";
		cout << endl;
	}
	cout << "********************************************" << endl;
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
		{
			img.at<Point3f>(row, col).x = static_cast<float>(row + col);
			img.at<Point3f>(row, col).y = static_cast<float>(row - col);
			img.at<Point3f>(row, col).z = static_cast<float>(row * col);
		}
	}
	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col)
			cout << img.at<Vec3f>(row, col) << "  ";
		cout << endl;
	}
	//
	Mat res;
	reduce(img, res, 0, CV_REDUCE_SUM);
	cout << "res size : " << res.size() << endl;
	cout << "res depth : " << res.depth() << endl;
	cout << "res channel : " << res.channels() << endl;
	cout << "res row : " << res.rows << endl;
	cout << "res col : " << res.cols << endl;
	cout << "res dim : " << res.dims << endl;
	cout << "res size width : " << res.size().width << endl;
	cout << "res size height : " << res.size().height << endl;
	cout << "**************************************************" << endl;
	auto pres = res.ptr<float>(0);
	for (int j = 0; j < res.channels()*res.cols; ++j)
		cout << j << " : " << pres[j] << endl;
	cout << "**************************************************" << endl;
	for (int i = 0; i < res.rows; ++i) {
		for (int j = 0; j < res.cols; ++j)
			cout << res.at<Vec3f>(i, j) << "  ";
		cout << endl;
	}
	//
	char c;
	cin >> c;
	return 0;
}
