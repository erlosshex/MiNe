#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
using namespace cv;

const string imagePath("F:\\Documentations\\2018\\09\\25\\test1.jpg");

void GetGammaTransLUT(uchar *pLUT, float Gamma, int iLUTLen)
{
	for (int i = 0; i<iLUTLen; i++)
	{
		pLUT[i] = (uchar)(pow((float)i / 255, Gamma) * 255);
	}
}
void GammaTrans(uchar *pSrc, uchar *pDst, const int iHeight,
	const int iWidth, float Gamma)
{
	uchar *pLUT = new uchar[256];
	GetGammaTransLUT(pLUT, Gamma, 256);
	for (int i = 0; i<iHeight*iWidth; i++)
	{
		pDst[i] = (uchar)pLUT[pSrc[i]];
	}
	delete[]pLUT;
}

void GetInvertLUT(uchar *pLUT, int iLUTLen) {
	for (int i = 0; i < iLUTLen; ++i) {
		pLUT[i] = 255 - i;
	}
}

void InvertTrans(uchar *pSrc, uchar *pDst, const int iHeight, const int iWidth) {
	uchar *pLUT = new uchar[256];
	GetInvertLUT(pLUT, 256);
	for (int i = 0; i<iHeight*iWidth; i++)
	{
		pDst[i] = (uchar)pLUT[pSrc[i]];
	}
	delete[]pLUT;
}

double GetThresMean(uchar *src, const int h, int w, double thres1, double thres2) {
	double s = 0;
	int c = 0;
	for (int i = 0; i < h*w; ++i) {
		if (src[i] > thres1 && src[i] < thres2) {
			s += src[i];
			c++;
		}
	}
	if (c == 0) {
		return -1;
	}
	else {
		return (s / c);
	} 
}

int main(int argc, char **argv)
{
	Mat image = imread(imagePath, 0);
	namedWindow("origin gray image", CV_WINDOW_NORMAL);
	imshow("origin gray image", image);
	//
	Mat thresImage;
	threshold(image,thresImage,60,255,THRESH_BINARY_INV);
	namedWindow("gray thres image", CV_WINDOW_NORMAL);
	imshow("gray thres image", image);
	//
	Mat gammaImage(image.size(),image.type());
	const int iHeight = image.rows;
	const int iWidth = image.cols;
	uchar* pSrc = image.data;
	uchar* pDst = gammaImage.data;
	GammaTrans(pSrc, pDst, iHeight, iWidth, 2.7);
	namedWindow("gray gamma image", CV_WINDOW_NORMAL);
	imshow("gray gamma image", gammaImage);
	//
	Mat invertImage(gammaImage.size(), gammaImage.type());
	const int iHeight1 = gammaImage.rows;
	const int iWidth1 = gammaImage.cols;
	uchar* pSrc1 = gammaImage.data;
	uchar* pDst1 = invertImage.data;
	InvertTrans(pSrc1, pDst1, iHeight1, iWidth1);
	namedWindow("gamma invert image", CV_WINDOW_NORMAL);
	imshow("gamma invert image", invertImage);
	//
	Mat thresImage1;
	threshold(invertImage, thresImage1, 210, 255, THRESH_BINARY);
	namedWindow("invert thres image", CV_WINDOW_NORMAL);
	imshow("invert thres image", thresImage1);
	//
	Mat dst;
	image.copyTo(dst,thresImage1);
	namedWindow("thres1 mask image", CV_WINDOW_NORMAL);
	imshow("thres1 mask image", dst);
	//
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat drawImage(dst.size(),dst.type(),Scalar(255));
	Scalar color(0);
	findContours(dst,contours,hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); ++i) {
		Moments moms = moments(Mat(contours[i]));
		double area = moms.m00;
		if (area > 3000 && area < 10000) {
			Rect bRect = boundingRect(Mat(contours[i]));
			Mat tempROI = image(bRect);
			double thresM = GetThresMean(tempROI.data, tempROI.rows, tempROI.cols, 50, 200);
			//Scalar meanR;
			//Scalar stdR;
			//meanStdDev(tempROI,meanR,stdR);
			//cout << "mean pixel : " << meanR[0] << endl;
			cout << "thresM pixel : " << thresM << endl;
			if( thresM>50 && thresM<150)  // this is important
				drawContours(drawImage, contours, i, color, CV_FILLED, 8, hierarchy);
		}
		
	}
	namedWindow("mask contours image", CV_WINDOW_NORMAL);
	imshow("mask contours image", drawImage);
	//
	Mat resMask(drawImage.size(), drawImage.type(), Scalar(0));
	InvertTrans(drawImage.data, resMask.data, drawImage.rows, drawImage.cols);
	Mat res;
	image.copyTo(res, resMask);
	namedWindow("res mask image", CV_WINDOW_NORMAL);
	imshow("res mask image", res);
	//
	waitKey();
	destroyAllWindows();
	return 0;
}
