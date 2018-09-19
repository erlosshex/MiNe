//此是网上旋转的两种代码，海涛
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;

double cvMatImageSkewAngleUsingSHT(cv::Mat gray)
{
	// Load in grayscale.
	//cv::Mat src = cv::imread(filename, cv::IMREAD_GRAYSCALE);

	if (gray.data == NULL) {
		fprintf(stderr, "error: src.data == NULL\n");
		return 0.0f;
	}
	if (gray.channels() != 1) {
		fprintf(stderr, "error: src.channels() != 1\n");
		return 0.0f;
	}
	cv::bitwise_not(gray, gray);

	cv::Size size = gray.size();

	// 需要的是黑色背景
	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(gray, lines, 1, CV_PI / 180, 100, size.width / 2.f, 20);


	cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
	double angle = 0.;
	unsigned nb_lines = lines.size();
	for (unsigned i = 0; i < nb_lines; ++i)
	{
		cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0, 0));
		angle += atan2((double)lines[i][3] - lines[i][1], (double)lines[i][2] - lines[i][0]);
	}
	angle /= nb_lines; // mean angle, in radians.
	angle = angle * 180 / CV_PI;
	cv::imwrite("E:\\Project\\C085\\Codes\\test\\image\\test11.bmp", disp_lines);

	fprintf(stderr, "ImageSkewAngle result: %f\n", angle);

	return angle;
}

cv::Mat cvMatImageDeskew(cv::Mat src, double angle)
{
	cv::Mat img = src.clone();
	cv::imwrite("E:\\Project\\C085\\Codes\\test\\image\\test21.bmp", img);

	cv::Mat cropped;
	cv::Mat rotated;
	// Load in grayscale.
	//cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE);
	if (img.data == NULL) {
		fprintf(stderr, "error: src.data == NULL");
		return rotated;
	}
	if (img.channels() != 1) {
		fprintf(stderr, "error: src.channels() != 1");
		return rotated;
	}

	std::vector<cv::Point> points;
	cv::Mat_<uchar>::iterator it = img.begin<uchar>();
	cv::Mat_<uchar>::iterator end = img.end<uchar>();
	for (; it != end; ++it)
		if (*it)
			points.push_back(it.pos());

	cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));

	cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, angle, 1);

	// 旋转
	cv::warpAffine(img, rotated, rot_mat, img.size(), cv::INTER_CUBIC); // 默认是黑色背景
																		//cv::warpAffine(img, rotated, rot_mat, img.size(), CV_INTER_CUBIC, cv::BORDER_CONSTANT, cvScalarAll(0)); // 黑色背景
																		//cv::warpAffine(img, rotated, rot_mat, img.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, cvScalar(255, 255, 255)); // 白色背景

	cv::imwrite("E:\\Project\\C085\\Codes\\test\\image\\test31.bmp", rotated);
	if (0) {
		// 切割
		cv::Size box_size = box.size;
		if (box.angle < -45.)
			std::swap(box_size.width, box_size.height);
		cv::getRectSubPix(rotated, box_size, box.center, cropped);
		cv::imwrite("E:\\Project\\C085\\Codes\\test\\image\\test41.bmp", cropped);
	}

	return rotated;

}


int mySum(int n1, int n2) {
	return n1 + n2;
}

// 参考 Félix Abecassis - OpenCV - Bounding Box & Skew Angle
// http://felix.abecassis.me/2011/10/opencv-bounding-box-skew-angle/
double cvMatImageSkewAngleWithImageBin(cv::Mat image_bin)
{
	// 输入的是黑底白字图片
	cv::Mat image = image_bin.clone();
	if (image.data == NULL) {
		fprintf(stderr, "error: src.data1 == NULL\n");
		return 0.0f;
	}
	if (image.channels() != 1) {
		fprintf(stderr, "error: src.channels()1 != 1\n");
		return 0.0f;
	}
	cv::imwrite("E:\\Project\\C085\\Codes\\test\\image\\test211.bmp", image);

	// Size(23, 3)); Size(5, 3));，使用Size(3, 3)最好
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	// 最基本的形态学操作有二：腐蚀与膨胀(Erosion 与 Dilation)
	cv::erode(image, image, element);

	cv::imwrite("E:\\Project\\C085\\Codes\\test\\image\\test221.bmp", image);

	std::vector<cv::Point> points;
	cv::Mat_<uchar>::iterator it = image.begin<uchar>();
	cv::Mat_<uchar>::iterator end = image.end<uchar>();
	for (; it != end; ++it)
		if (*it)
			points.push_back(it.pos());

	cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));


	double angle = box.angle;
	if (angle < -45.)
		angle += 90.;

	if (1) {
		cv::Point2f vertices[4];
		box.points(vertices);
		for (int i = 0; i < 4; ++i)
			cv::line(image, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 0, 0), 1, CV_AA);
		cv::imwrite("E:\\Project\\C085\\Codes\\test\\image\\test231.bmp", image);

	}


	fprintf(stderr, "ImageSkewAngle result: %f\n", angle);

	return angle;
}

cv::Mat cvMatImageSkewAngleWithImageGray(cv::Mat image_gray)
{
	// 输入的是灰度图片
	// Load in grayscale.
	//cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE);

	cv::Mat image = image_gray.clone();
	if (image.data == NULL) {
		fprintf(stderr, "error: src.data == NULL\n");
		return (cv::Mat());
	}
	if (image.channels() != 1) {
		fprintf(stderr, "error: src.channels() != 1\n");
		return (cv::Mat());
	}

	//中值滤波器是一种非线性滤波器，常用于消除图像中的椒盐噪声。与低通滤波不同的是，中值滤波有利于保留边缘的尖锐度，但它会洗去均匀介质区域中的纹理。
	//cv::medianBlur(img, img, 3);

	//cv::GaussianBlur(img, img, cv::Size(3, 3), 0);
	//GaussianBlur(img, img, cvSize(11,11), 0);//change from median blur to gaussian for more accuracy of square detection

	// Binarize
	//cv::threshold(img, img, 225, 255, CV_THRESH_BINARY_INV);
	cv::threshold(image, image, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY); // 二值化效果好
	namedWindow("threshold image", cv::WINDOW_NORMAL);
	cv::imshow("threshold image",image);
	cv::waitKey();
	return image;
}


int main(int argc, char **argv)
{
	string path1{"E:\\Project\\C085\\Codes\\test\\image\\test01.bmp"};
	cv::Mat m = cv::imread(path1,cv::IMREAD_GRAYSCALE);
	cv::Mat mn = cv::imread(path1, cv::IMREAD_GRAYSCALE);
	double angle;
	angle = cvMatImageSkewAngleUsingSHT(m);
	cvMatImageDeskew(m, angle);
	//
	cv::Mat m1 = imread(path1, cv::IMREAD_GRAYSCALE);
	cv::Mat mm;
	cv::Mat mmm;
	cv::bitwise_not(m1, mmm);
	mm = cvMatImageSkewAngleWithImageGray(mmm);
	angle = cvMatImageSkewAngleWithImageBin(mm);
	cvMatImageDeskew(mn, angle);
	return 0;
}
