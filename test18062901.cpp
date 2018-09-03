#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void deal2DPicture() {
	std::cout << "Hello world" << std::endl;
	cv::Mat img = cv::imread("E:\\cheka.bmp");
	cv::imshow("H", img);
	cv::Mat m = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	cv::Mat dst;
	cv::erode(img, dst, m);
	cv::dilate(dst, dst, m);
	cv::imshow("S", dst);
	cv::blur(img, dst, cv::Size(7, 7));
	cv::imshow("S1", dst);
	/**********************************************************************/
	//Canny algorithms
	cv::Mat dstImage, edge, grayImage;
	dstImage.create(img.size(), img.type());
	cv::cvtColor(img, grayImage, CV_BGR2GRAY);
	cv::Mat mm = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	cv::erode(grayImage, grayImage, mm);
	cv::dilate(grayImage, grayImage, mm);
	cv::blur(grayImage, edge, cv::Size(3, 3));
	cv::Canny(edge, edge, 3, 70, 3);
	cv::imshow("edge", edge);
	cv::waitKey(0);
}

void deal2DPictureFromVideo(cv::Mat imgFromVideo) {
	cv::Mat img = imgFromVideo;
	/**********************************************************************/
	//Canny algorithms
	cv::Mat dstImage, edge, grayImage;
	dstImage.create(img.size(), img.type());
	cv::cvtColor(img, grayImage, CV_BGR2GRAY);
	cv::Mat mm = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	cv::erode(grayImage, grayImage, mm);
	cv::dilate(grayImage, grayImage, mm);
	cv::blur(grayImage, edge, cv::Size(3, 3));
	cv::Canny(edge, edge, 3, 70, 3);
	cv::imshow("edge", edge);
	cv::waitKey(0);
}

void dealVideoPicture() {
	using namespace cv;
	using namespace std;

	cout << "Hello world" << endl;

	VideoCapture vp;
	vp.open(0);

	while (true) {
		Mat frame;
		vp >> frame;
		imshow("Video", frame);
		int keyNum = 0;
		keyNum = waitKey(30);
		if (keyNum == 27) {
			deal2DPictureFromVideo(frame);
			break;
		}
		/*
		if (keyNum == 27) {
		deal2DPictureFromVideo(frame);
		break;
		}
		*/
	}
}

int main(int argc, char **argv)
{
	dealVideoPicture();
	return 0;
}
