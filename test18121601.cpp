// *head file* //
#pragma once
#ifndef _DETECTEDGES_H
#define _DETECTEDGES_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include<exception>
#include<cmath>
#include<algorithm>
#include<utility>

using namespace std;
using namespace cv;

class DetectEdges10 {
private:
	//
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	vector<vector<Point> > contoursLR;
	vector<Vec4i> hierarchyLR;
	vector<vector<Point> > contoursUD;
	vector<Vec4i> hierarchyUD;
	vector<vector<Point> > contoursU;
	vector<Vec4i> hierarchyU;
	vector<vector<Point> > contoursD;
	vector<Vec4i> hierarchyD;

	Mat dealImage;
	Mat dealImage1;
	Rect dealBox;

	double deltaLen;

public:

	uchar lut2DUp[256];
	uchar lut2DDown[256];
	uchar lut2DLR[256];

	uchar lut5SCenter[256];
	uchar lut5SUp[256];
	uchar lut5SDown[256];
	uchar lut5SLeft[256];
	uchar lut5SRight[256];

	Point2f resVertices[4];

public:
	//

	void setDaltaLen(double d);
	
	void setImageAndROI(Mat &img, Rect &roi);

	void initilize();

	// 2D

	double dealOneCenterRight121102();

	// 5S

	void dealFiveCenter121501();

	double dealFiveUp();

	double dealFiveDown();

	void dealFiveLeft();

	void dealFiveRight();

private:
	// utilities
	void minMaxX(Mat *m, double *minX, double *minY, double *maxX, double *maxY);

	void getCrossPoint(Vec4f *l1, Vec4f *l2, Point2f *p);

	void convertLinePoint2Vec(Point2f *p1, Point2f *p2, Vec4f *l);

	void moveLine(Vec4f *l, float distX, float distY);

	bool equalPointPixel(Mat img, Point2f *p, uchar pixel);

	vector<Point2f> getLineSegmentPoint(Point2f *p1, Point2f *p2);

	bool judgeLine(Mat img, Vec4f *l, Vec4f *l1, Vec4f *l2, uchar pixel, float percent, int thres = 2);


	// 2018.12.07  adding
	vector<uchar> filterPointSetPixel(vector<uchar> &pv, double percent);

	// 2018.12.06  adding
	void calcPixelStatistic(vector<uchar> &pv, double *meanPixel, double *stddev, double *maxPixel, double *minPixel);

	// 2018.12.06  adding
	bool judgeLinePixelStatistic(Mat img, Vec4f *l, Vec4f *l1, Vec4f *l2, double percent, double stddev, double thres, double maxMinPixelDist = -1);

	// 2018.12.06  adding
	bool judgeLinePixelGrad(Mat img, Vec4f *l, Vec4f *l1, Vec4f *l2, double thres, int oriention, int step, double percent, int selec = 0);

	// 2018.12.07  adding
	Vec4f selectLineFromRegionGrad(Mat img, Vec4f *l1, Vec4f *l2, Vec4f *l3, Vec4f *l4, double thres, int oriention, double percent, bool PN);

	Vec4f selectLineFromRegionStatistic(Mat img, Vec4f *l1, Vec4f *l2, Vec4f *l3, Vec4f *l4, double thres, int oriention, double percent, bool PN);

	// 2018.12.08  adding
	Point2f getCenterPointOfThreeLines(Vec4f *l, Vec4f *l1, Vec4f *l2);

	// 2018.12.11  adding
	bool judgeCrossWithThreeLines(Mat img, Vec4f *l, Vec4f *l1, Vec4f *l2, uchar pixel1, uchar pixel2, int sz, double percent);

	// 2019.12.16  adding
	void copyUcharArr(uchar *src, uchar *dst, int len);
};

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// *source file* //

#include "DetectEdges10.h"


void DetectEdges10::setDaltaLen(double d) {
	deltaLen = d;
}

void DetectEdges10::setImageAndROI(Mat &img, Rect &roi) {
	dealImage = img.clone();
	dealBox = roi;
}

void DetectEdges10::initilize() {
	// 2D Down
	for (int i = 0; i < 256; ++i) {
		if (i < 100) {
			lut2DDown[i] = i;
		}
		else if (i < 150) {
			lut2DDown[i] = i - 50;
		}
		else if (i < 200) {
			lut2DDown[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut2DDown[i] = n;
			else
				lut2DDown[i] = n - 255;
		}
	}

	// 2D Up
	for (int i = 0; i < 256; ++i) {
		if (i < 100) {
			lut2DUp[i] = i;
		}
		else if (i < 150) {
			lut2DUp[i] = i - 50;
		}
		else if (i < 200) {
			lut2DUp[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut2DUp[i] = n;
			else
				lut2DUp[i] = n - 255;
		}
	}

	// 2D Left and Right
	for (int i = 0; i < 256; ++i) {
		if (i < 150) {
			lut2DLR[i] = 0;
		}
		else if (i <= 200) {
			lut2DLR[i] = 255;
		}
		else {
			int n = i + 50;
			if (n > 240) {
				lut2DLR[i] = 0;
			}
			else {
				lut2DLR[i] = n;
			}
		}
	}

	// 5S Center
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut5SCenter[i] = 0;
		}
		else if (i < 100) {
			lut5SCenter[i] = 0;
		}
		else if (i < 150) {
			lut5SCenter[i] = i - 50;
		}
		else if (i < 200) {
			lut5SCenter[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut5SCenter[i] = n;
			else
				lut5SCenter[i] = n - 255;
		}
	}

	// 5S Up
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut5SUp[i] = 0;
		}
		else if (i < 100) {
			lut5SUp[i] = 0;
		}
		else if (i < 150) {
			lut5SUp[i] = i - 50;
		}
		else if (i < 200) {
			lut5SUp[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut5SUp[i] = n;
			else
				lut5SUp[i] = n - 255;
		}
	}

	// 5S Down
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut5SDown[i] = 255;
		}
		else if (i < 100) {
			lut5SDown[i] = 0;
		}
		else if (i < 150) {
			lut5SDown[i] = i - 50;
		}
		else if (i < 200) {
			lut5SDown[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut5SDown[i] = n;
			else
				lut5SDown[i] = n - 255;
		}
	}

	// 5S Left
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut5SLeft[i] = 255;
		}
		else if (i < 100) {
			lut5SLeft[i] = 0;
		}
		else if (i < 150) {
			lut5SLeft[i] = i - 50;
		}
		else if (i < 200) {
			lut5SLeft[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut5SLeft[i] = n;
			else
				lut5SLeft[i] = n - 255;
		}
	}

	// 5S Right
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut5SRight[i] = 255;
		}
		else if (i < 100) {
			lut5SRight[i] = 0;
		}
		else if (i < 150) {
			lut5SRight[i] = i - 50;
		}
		else if (i < 200) {
			lut5SRight[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut5SRight[i] = n;
			else
				lut5SRight[i] = n - 255;
		}
	}
	
	deltaLen = 0;
}

/*********************** detect edges *****************************/

// 2D

double DetectEdges10::dealOneCenterRight121102() {
	// 这个版本是要将原先的二值化后的表面进行白色填充，然后进行处理

	// 这里依赖一个dealImage的Mat其他用到的都是全局变量，dealImage是传进来的
	// 基本上是ok的
	int xlimit = dealBox.width;
	int ylimit = dealBox.height;

	uchar lut[256];  // for down
	copyUcharArr(lut2DDown, lut);

	/*for (int i = 0; i < 256; ++i) {
		if (i < 100) {
			lut[i] = i;
		}
		else if (i < 150) {
			lut[i] = i - 50;
		}
		else if (i < 200) {
			lut[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut[i] = n;
			else
				lut[i] = n - 255;
		}
	}*/

	uchar lut1[256];  // for left and right
	copyUcharArr(lut2DLR, lut1);

	/*for (int i = 0; i < 256; ++i) {
		if (i < 150) {
			lut1[i] = 0;
		}
		else if (i <= 200) {
			lut1[i] = 255;
		}
		else {
			int n = i + 50;
			if (n > 240) {
				lut1[i] = 0;
			}
			else {
				lut[i] = n;
			}
		}
	}
*/

	uchar lut2[256];  // for up
	copyUcharArr(lut2DUp, lut2);

	/*for (int i = 0; i < 256; ++i) {
		if (i < 100) {
			lut2[i] = i;
		}
		else if (i < 150) {
			lut2[i] = i - 50;
		}
		else if (i < 200) {
			lut2[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut2[i] = n;
			else
				lut2[i] = n - 255;
		}
	}*/

	dealImage = dealImage(dealBox);
	dealImage.convertTo(dealImage, CV_8UC1);
	double t1 = (double)getTickCount();
	/*********************left and right***************************/

	Mat dealImageLR = dealImage.clone();
	uchar *p;
	for (int r = 0; r < dealImageLR.rows; ++r) {
		p = dealImageLR.ptr<uchar>(r);
		for (int c = 0; c < dealImageLR.cols; ++c) {
			p[c] = lut1[p[c]];
		}
	}

	for (int r = 1; r < dealImageLR.rows - 1; ++r) {
		p = dealImageLR.ptr<uchar>(r);
		uchar *preP = dealImageLR.ptr<uchar>(r - 1);
		uchar *nextP = dealImageLR.ptr<uchar>(r + 1);
		for (int c = 1; c < dealImageLR.cols - 1; ++c) {
			int flag1 = 0;
			int flag2 = 0;
			int flag3 = 0;
			int flag4 = 0;
			if (p[c - 1] == 255)
				flag1 = 1;
			if (preP[c] == 255)
				flag2 = 1;
			if (p[c + 1] == 255)
				flag3 = 1;
			if (nextP[c] == 255)
				flag4 = 1;
			if ((flag1 + flag2 + flag3 + flag4) < 2)
				p[c] = 0;
		}
	}
	imshow("dealImageLR", dealImageLR);
	Mat maskLR = dealImageLR.clone();

	Mat maskLRCopy = maskLR.clone();

	imshow("maskLR", maskLR);

	Mat kernel1LR = getStructuringElement(MorphTypes::MORPH_ERODE, Size(3, 3));
	Mat kernel2LR = getStructuringElement(MorphTypes::MORPH_DILATE, Size(3, 3));
	erode(maskLR, maskLR, kernel1LR, Point(-1, -1), 1);
	dilate(maskLR, maskLR, kernel2LR, Point(-1, -1), 1);
	erode(maskLR, maskLR, kernel1LR, Point(-1, -1), 1);
	dilate(maskLR, maskLR, kernel2LR, Point(-1, -1), 1);

	imshow("deal maskLR", maskLR);
	RotatedRect rectLR;

	findContours(maskLR, contoursLR, hierarchyLR, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	double maxAreaLR = 0;
	int maxIdxLR = -1;
	for (int contourIdx = 0; contourIdx < contoursLR.size(); ++contourIdx) {
		double area = contourArea(contoursLR[contourIdx]);
		if (area > maxAreaLR) {
			maxAreaLR = area;
			maxIdxLR = contourIdx;
		}
	}

	rectLR = minAreaRect(contoursLR[maxIdxLR]);

	Point2f verticesLR[4];
	rectLR.points(verticesLR);

	Point2f tempVerticesLR[4];
	float verticesMeanXLR = 0;
	float verticesMeanYLR = 0;
	for (int i = 0; i < 4; ++i) {
		tempVerticesLR[i] = verticesLR[i];
		verticesMeanXLR += verticesLR[i].x;
		verticesMeanYLR += verticesLR[i].y;
	}

	verticesMeanXLR = verticesMeanXLR / 4;
	verticesMeanYLR = verticesMeanYLR / 4;

	for (int i = 0; i < 4; ++i) {
		if (tempVerticesLR[i].x<verticesMeanXLR && tempVerticesLR[i].y>verticesMeanYLR) {
			verticesLR[0] = tempVerticesLR[i];
			continue;
		}
		if (tempVerticesLR[i].x < verticesMeanXLR && tempVerticesLR[i].y < verticesMeanYLR) {
			verticesLR[1] = tempVerticesLR[i];
			continue;
		}
		if (tempVerticesLR[i].x > verticesMeanXLR && tempVerticesLR[i].y < verticesMeanYLR) {
			verticesLR[2] = tempVerticesLR[i];
			continue;
		}
		if (tempVerticesLR[i].x > verticesMeanXLR && tempVerticesLR[i].y > verticesMeanYLR) {
			verticesLR[3] = tempVerticesLR[i];
			continue;
		}
	}

	Vec4f l1LR, l2LR, l3LR, l4LR;
	convertLinePoint2Vec(&verticesLR[0], &verticesLR[1], &l1LR);
	convertLinePoint2Vec(&verticesLR[1], &verticesLR[2], &l2LR);
	convertLinePoint2Vec(&verticesLR[2], &verticesLR[3], &l3LR);
	convertLinePoint2Vec(&verticesLR[3], &verticesLR[0], &l4LR);

	imshow("maskLRCopy", maskLRCopy);

	int moveCountLR = 0;
	while (!judgeLine(maskLRCopy, &l4LR, &l1LR, &l3LR, 255, 0.4) && moveCountLR < 40) {
		moveLine(&l4LR, 0, -2);
		moveCountLR++;
	}

	moveCountLR = 0;
	while (!judgeLine(maskLRCopy, &l1LR, &l2LR, &l4LR, 255, 0.4) && moveCountLR < 40) {
		moveLine(&l1LR, 2, 0);
		moveCountLR++;
	}

	moveCountLR = 0;
	while (!judgeLine(maskLRCopy, &l2LR, &l1LR, &l3LR, 255, 0.2) && moveCountLR < 40) {
		moveLine(&l2LR, 0, 2);
		moveCountLR++;
	}

	moveCountLR = 0;
	while (!judgeLine(maskLRCopy, &l3LR, &l2LR, &l4LR, 255, 0.4) && moveCountLR < 40) {
		moveLine(&l3LR, -2, 0);
		moveCountLR++;
	}

	for (int i = 0; i < 4; ++i) {
		line(maskLR, verticesLR[i], verticesLR[(i + 1) % 4], Scalar(255));
	}

	/*********************left and right end***********************/

	/*********************up***************************************/

	Mat dealImageU = dealImage.clone();

	for (int r = 0; r < dealImageU.rows; ++r) {
		p = dealImageU.ptr<uchar>(r);
		for (int c = 0; c < dealImageU.cols; ++c) {
			p[c] = lut2[lut2[p[c]]];
		}
	}

	double maxAreaU = 0;
	int maxIdxU = -1;

	imshow("dealImageU", dealImageU);
	Mat maskU = dealImageU > 200;
	Mat drawImageU(dealImageU.size(), CV_8UC1, Scalar(0));

	findContours(maskU, contoursU, hierarchyU, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	for (int contourIdx = 0; contourIdx < contoursU.size(); ++contourIdx) {
		double area = contourArea(contoursU[contourIdx]);
		if (area > maxAreaU) {
			maxAreaU = area;
			maxIdxU = contourIdx;
		}
	}

	drawContours(drawImageU, contoursU, maxIdxU, Scalar(255), CV_FILLED);

	maskU = drawImageU;

	imshow("maskU", maskU);
	Mat maskUCopy = maskU.clone();

	Mat kernel1U = getStructuringElement(MorphTypes::MORPH_ERODE, Size(3, 3));
	Mat kernel2U = getStructuringElement(MorphTypes::MORPH_DILATE, Size(3, 3));
	dilate(maskU, maskU, kernel2U, Point(-1, -1), 1);
	erode(maskU, maskU, kernel1U, Point(-1, -1), 1);

	erode(maskU, maskU, kernel1U, Point(-1, -1), 1);
	dilate(maskU, maskU, kernel2U, Point(-1, -1), 1);

	erode(maskU, maskU, kernel1U, Point(-1, -1), 1);
	dilate(maskU, maskU, kernel2U, Point(-1, -1), 1);

	//erode(maskU, maskU, kernel1U, Point(-1, -1), 2);
	imshow("deal maskU", maskU);
	RotatedRect rectU;

	findContours(maskU, contoursU, hierarchyU, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);


	maxAreaU = 0;
	maxIdxU = -1;

	for (int contourIdx = 0; contourIdx < contoursU.size(); ++contourIdx) {
		double area = contourArea(contoursU[contourIdx]);
		if (area > maxAreaU) {
			maxAreaU = area;
			maxIdxU = contourIdx;
		}
	}

	rectU = minAreaRect(contoursU[maxIdxU]);

	Point2f verticesU[4];
	rectU.points(verticesU);

	Point2f tempVerticesU[4];
	float verticesMeanXU = 0;
	float verticesMeanYU = 0;
	for (int i = 0; i < 4; ++i) {
		tempVerticesU[i] = verticesU[i];
		verticesMeanXU += verticesU[i].x;
		verticesMeanYU += verticesU[i].y;
	}

	verticesMeanXU = verticesMeanXU / 4;
	verticesMeanYU = verticesMeanYU / 4;

	for (int i = 0; i < 4; ++i) {
		if (tempVerticesU[i].x<verticesMeanXU && tempVerticesU[i].y>verticesMeanYU) {
			verticesU[0] = tempVerticesU[i];
			continue;
		}
		if (tempVerticesU[i].x < verticesMeanXU && tempVerticesU[i].y < verticesMeanYU) {
			verticesU[1] = tempVerticesU[i];
			continue;
		}
		if (tempVerticesU[i].x > verticesMeanXU && tempVerticesU[i].y < verticesMeanYU) {
			verticesU[2] = tempVerticesU[i];
			continue;
		}
		if (tempVerticesU[i].x > verticesMeanXU && tempVerticesU[i].y > verticesMeanYU) {
			verticesU[3] = tempVerticesU[i];
			continue;
		}
	}

	Vec4f l1U, l2U, l3U, l4U;
	l1U = l1LR;
	convertLinePoint2Vec(&verticesU[1], &verticesU[2], &l2U);
	l3U = l3LR;

	int moveCountU = 0;


	// line 2
	cout << "current line : " << "down line 2" << endl;
	imshow("maskUCopy", maskUCopy);
	moveCountU = 0;
	while (!judgeLine(maskUCopy, &l2U, &l1U, &l3U, 255, 0.7) && moveCountU < 60) {
		moveLine(&l2U, 0, 1);
		moveCountU++;
	}



	for (int i = 0; i < 4; ++i) {
		line(maskU, verticesU[i], verticesU[(i + 1) % 4], Scalar(255));
	}

	/*********************up end***********************************/


	/*********************down*************************************/

	Mat dealImageD = dealImage.clone();

	for (int r = 0; r < dealImageD.rows; ++r) {
		p = dealImageD.ptr<uchar>(r);
		for (int c = 0; c < dealImageD.cols; ++c) {
			p[c] = lut[lut[p[c]]];
		}
	}

	double maxAreaD = 0;
	int maxIdxD = -1;

	imshow("dealImageD", dealImageD);
	Mat maskD = dealImageD > 200;
	Mat drawImageD(dealImageD.size(), CV_8UC1, Scalar(0));

	findContours(maskD, contoursD, hierarchyD, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	for (int contourIdx = 0; contourIdx < contoursD.size(); ++contourIdx) {
		double area = contourArea(contoursD[contourIdx]);
		if (area > maxAreaD) {
			maxAreaD = area;
			maxIdxD = contourIdx;
		}
	}

	drawContours(drawImageD, contoursD, maxIdxD, Scalar(255), CV_FILLED);

	maskD = drawImageD;

	imshow("maskD", maskD);
	Mat maskDCopy = maskD.clone();

	Mat kernel1D = getStructuringElement(MorphTypes::MORPH_ERODE, Size(3, 3));
	Mat kernel2D = getStructuringElement(MorphTypes::MORPH_DILATE, Size(3, 3));
	dilate(maskD, maskD, kernel2D, Point(-1, -1), 1);
	erode(maskD, maskD, kernel1D, Point(-1, -1), 1);

	erode(maskD, maskD, kernel1D, Point(-1, -1), 1);
	dilate(maskD, maskD, kernel2D, Point(-1, -1), 1);
	imshow("deal maskD", maskD);
	RotatedRect rectD;

	findContours(maskD, contoursD, hierarchyD, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	maxAreaD = 0;
	maxIdxD = -1;

	for (int contourIdx = 0; contourIdx < contoursD.size(); ++contourIdx) {
		double area = contourArea(contoursD[contourIdx]);
		if (area > maxAreaD) {
			maxAreaD = area;
			maxIdxD = contourIdx;
		}
	}


	rectD = minAreaRect(contoursD[maxIdxD]);

	Point2f verticesD[4];
	rectD.points(verticesD);

	Point2f tempVerticesD[4];
	float verticesMeanXD = 0;
	float verticesMeanYD = 0;
	for (int i = 0; i < 4; ++i) {
		tempVerticesD[i] = verticesD[i];
		verticesMeanXD += verticesD[i].x;
		verticesMeanYD += verticesD[i].y;
	}

	verticesMeanXD = verticesMeanXD / 4;
	verticesMeanYD = verticesMeanYD / 4;

	for (int i = 0; i < 4; ++i) {
		if (tempVerticesD[i].x<verticesMeanXD && tempVerticesD[i].y>verticesMeanYD) {
			verticesD[0] = tempVerticesD[i];
			continue;
		}
		if (tempVerticesD[i].x < verticesMeanXD && tempVerticesD[i].y < verticesMeanYD) {
			verticesD[1] = tempVerticesD[i];
			continue;
		}
		if (tempVerticesD[i].x > verticesMeanXD && tempVerticesD[i].y < verticesMeanYD) {
			verticesD[2] = tempVerticesD[i];
			continue;
		}
		if (tempVerticesD[i].x > verticesMeanXD && tempVerticesD[i].y > verticesMeanYD) {
			verticesD[3] = tempVerticesD[i];
			continue;
		}
	}

	Vec4f l1D, l2D, l3D, l4D;
	l1D = l1LR;
	l3D = l3LR;
	convertLinePoint2Vec(&verticesD[3], &verticesD[0], &l4D);

	int moveCountD = 0;

	// line 4
	cout << "current line : " << "down line 4" << endl;
	imshow("maskDCopy", maskDCopy);

	moveCountD = 0;
	while ((!judgeCrossWithThreeLines(maskDCopy, &l4D, &l1D, &l3D, 255, 255, 7, 0.01) || !judgeLine(maskDCopy, &l4D, &l1D, &l3D, 255, 0.7)) && moveCountD < 60) {
		moveLine(&l4D, 0, -1);
		moveCountD++;
	}

	/*********************down end*********************************/

	Point2f vertices[4];

	getCrossPoint(&l4D, &l1LR, &vertices[0]);
	getCrossPoint(&l1LR, &l2U, &vertices[1]);
	getCrossPoint(&l2U, &l3LR, &vertices[2]);
	getCrossPoint(&l3LR, &l4D, &vertices[3]);

	double t2 = (double)getTickCount();

	cout << "Time : " << (t2 - t1) / getTickFrequency() << endl;

	for (int i = 0; i < 4; ++i) {
		line(dealImage, vertices[i], vertices[(i + 1) % 4], Scalar(0));
	}

	//imgCount++;

	imshow("test Image", dealImage);

	//imwrite(preSavePath + "\\" + to_string(imgIndex) + "_" + to_string(imgCount) + ".bmp", dealImage);

	return ((t2 - t1) / getTickFrequency());



}

// 5S

void DetectEdges10::dealFiveCenter121501() {
	// 这里依赖一个dealImage的Mat其他用到的都是全局变量，dealImage是传进来的
	// 基本上是ok的

	double areaSelectThreshold = 2000;


	uchar lut[256];
	copyUcharArr(lut5SCenter, lut);

	/*for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut[i] = 0;
		}
		else if (i < 100) {
			lut[i] = 0;
		}
		else if (i < 150) {
			lut[i] = i - 50;
		}
		else if (i < 200) {
			lut[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut[i] = n;
			else
				lut[i] = n - 255;
		}
	}*/

	uchar lut1[256];
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut1[i] = 0;
		}
		else {
			lut1[i] = 255;
		}
	}


	dealImage = dealImage(dealBox);
	dealImage.convertTo(dealImage, CV_8UC1);
	cout << "hello world" << endl;
	Mat showImage;
	showImage = dealImage.clone();

	//imwrite(preSavePath + "save.bmp", dealImage);
	cout << "save image : " << dealImage.rows << "x" << dealImage.cols << endl;


	imshow("处理前的图片", dealImage);

	uchar *p;
	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage.ptr<uchar>(r);
		for (int c = 0; c < dealImage.cols; ++c) {
			p[c] = lut[p[c]];
		}
	}


	imshow("处理的图片", dealImage);


	dealImage1 = dealImage.clone();
	//threshold(dealImage1, dealImage1, 230, 255, ThresholdTypes::THRESH_BINARY);

	//blur(dealImage1, dealImage1, Size(3, 3));
	//Mat grad1;
	//Mat grad2;
	//Sobel(dealImage1, grad1, CV_8U, 1, 0);
	//Sobel(dealImage1, grad2, CV_8U, 0, 1);
	//grad1 = cv::abs(grad1 - grad2);
	//dealImage1 = grad1;

	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage1.ptr<uchar>(r);
		for (int c = 0; c < dealImage1.cols; ++c) {
			p[c] = lut1[p[c]];
		}
	}

	imshow("处理的图片", dealImage1);

	Mat mask = dealImage1 > 100;

	imshow("刚得到的mask图片", mask);


	Mat imageCopy;
	dealImage1.copyTo(imageCopy, mask);

	Mat kernel1 = getStructuringElement(MorphTypes::MORPH_ERODE, Size(3, 3));
	Mat kernel2 = getStructuringElement(MorphTypes::MORPH_DILATE, Size(3, 3));
	dilate(mask, mask, kernel2, Point(-1, -1), 1);
	erode(mask, mask, kernel1, Point(-1, -1), 1);

	erode(mask, mask, kernel1, Point(-1, -1), 1);
	dilate(mask, mask, kernel2, Point(-1, -1), 1);


	//erode(mask, mask, kernel1, Point(-1, -1), 3);

	/*
	// one needs these codes
	erode(mask, mask, kernel1, Point(-1, -1), 1);
	// one end
	*/

	double minCol, minRow, maxCol, maxRow;
	minMaxX(&mask, &minCol, &minRow, &maxCol, &maxRow);
	cout << "min row : " << minRow << ", col : " << minCol << endl;
	cout << "max row : " << maxRow << ", col : " << maxCol << endl;


	imshow("腐蚀膨胀后的mask图片", mask);

	RotatedRect rect;


	findContours(mask, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);






	cout << "contours size : " << contours.size() << endl;
	cout << "dealing ..." << endl;

	vector<pair<int, double> > areaVector;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
		double area = contourArea(contours[contourIdx]);
		areaVector.push_back(make_pair(contourIdx, area));
	}
	sort(areaVector.begin(), areaVector.end(), [](pair<int, double> a, pair<int, double> b) {return (a.second>b.second);});

	vector<Point> needContour;

	needContour.insert(needContour.end(), contours[areaVector[0].first].begin(), contours[areaVector[0].first].end());
	//needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());

	cout << "max area : " << areaVector[0].second << endl;

	cout << "ci max area : " << areaVector[1].second << endl;
	if (contours.size() >= 2) {
		if (areaVector[1].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());
		}
	}
	else if (contours.size() >= 3) {
		if (areaVector[2].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[2].first].begin(), contours[areaVector[2].first].end());
		}
	}





	/*double maxArea = 0;
	int maxIdx = -1;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
	double area = contourArea(contours[contourIdx]);
	if (area > maxArea) {
	maxArea = area;
	maxIdx = contourIdx;
	}
	}

	rect = minAreaRect(contours[maxIdx]);*/

	rect = minAreaRect(needContour);

	Point2f vertices[4];
	rect.points(vertices);



	//////

	Point2f tempVertices[4];
	float verticesMeanX = 0;
	float verticesMeanY = 0;
	for (int i = 0; i < 4; ++i) {
		tempVertices[i] = vertices[i];
		verticesMeanX += vertices[i].x;
		verticesMeanY += vertices[i].y;
	}

	verticesMeanX = verticesMeanX / 4;
	verticesMeanY = verticesMeanY / 4;

	for (int i = 0; i < 4; ++i) {
		if (tempVertices[i].x<verticesMeanX && tempVertices[i].y>verticesMeanY) {
			vertices[0] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x < verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[1] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[2] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y > verticesMeanY) {
			vertices[3] = tempVertices[i];
			continue;
		}
	}

	//////






	for (int i = 0; i < 4; ++i) {
		cout << i << " : " << vertices[i].x << ", " << vertices[i].y << endl;
	}


	// five needs these codes
	Vec4f l1, l2, l3, l4;

	convertLinePoint2Vec(&vertices[0], &vertices[1], &l1);
	l1[2] = minCol;
	l1[3] = minRow;
	convertLinePoint2Vec(&vertices[1], &vertices[2], &l2);
	convertLinePoint2Vec(&vertices[2], &vertices[3], &l3);
	l3[2] = maxCol;
	l3[3] = maxRow;
	convertLinePoint2Vec(&vertices[3], &vertices[0], &l4);

	getCrossPoint(&l4, &l1, &vertices[0]);
	getCrossPoint(&l1, &l2, &vertices[1]);
	getCrossPoint(&l2, &l3, &vertices[2]);
	getCrossPoint(&l3, &l4, &vertices[3]);
	// five end


	for (int i = 0; i < 4; ++i) {
		line(dealImage1, vertices[i], vertices[(i + 1) % 4], Scalar(0));
		line(showImage, vertices[i], vertices[(i + 1) % 4], Scalar(0));
	}



	imshow("处理后的画图的图片", showImage);


	//imwrite(preSavePath + "dealImage1.bmp", dealImage1);
	//imwrite(preSavePath + "mask.bmp", mask);


	imshow("处理后的图片", dealImage1);
	cout << "dealImage1 : " << dealImage1.rows << "x" << dealImage1.cols << endl;


	imshow("处理后的Mask图片", mask);
	cout << "mask : " << mask.rows << "x" << mask.cols << endl;

	/*if (isSaveDealImage == true) {
		imwrite(preSavePath + "\\" + to_string(imgIndex) + "_" + to_string(imgCount) + ".bmp", showImage);
	}*/
}

double DetectEdges10::dealFiveUp() {
	// 这里依赖一个dealImage的Mat其他用到的都是全局变量，dealImage是传进来的
	// 基本上是ok的

	double areaSelectThreshold = 2000;


	uchar lut[256];
	copyUcharArr(lut5SUp, lut);

	/*for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut[i] = 0;
		}
		else if (i < 100) {
			lut[i] = 0;
		}
		else if (i < 150) {
			lut[i] = i - 50;
		}
		else if (i < 200) {
			lut[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut[i] = n;
			else
				lut[i] = n - 255;
		}
	}*/

	uchar lut1[256];
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut1[i] = 0;
		}
		else {
			lut1[i] = 255;
		}
	}


	dealImage = dealImage(dealBox);
	dealImage.convertTo(dealImage, CV_8UC1);
	cout << "hello world" << endl;
	Mat showImage;
	showImage = dealImage.clone();

	//imwrite(preSavePath + "save.bmp", dealImage);
	cout << "save image : " << dealImage.rows << "x" << dealImage.cols << endl;


	imshow("处理前的图片", dealImage);

	uchar *p;
	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage.ptr<uchar>(r);
		for (int c = 0; c < dealImage.cols; ++c) {
			p[c] = lut[p[c]];
		}
	}


	imshow("处理的图片", dealImage);


	dealImage1 = dealImage.clone();
	//threshold(dealImage1, dealImage1, 230, 255, ThresholdTypes::THRESH_BINARY);

	//blur(dealImage1, dealImage1, Size(3, 3));
	//Mat grad1;
	//Mat grad2;
	//Sobel(dealImage1, grad1, CV_8U, 1, 0);
	//Sobel(dealImage1, grad2, CV_8U, 0, 1);
	//grad1 = cv::abs(grad1 - grad2);
	//dealImage1 = grad1;

	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage1.ptr<uchar>(r);
		for (int c = 0; c < dealImage1.cols; ++c) {
			p[c] = lut1[p[c]];
		}
	}

	imshow("处理的图片", dealImage1);

	Mat mask = dealImage1 > 100;

	imshow("刚得到的mask图片", mask);


	Mat imageCopy;
	dealImage1.copyTo(imageCopy, mask);

	Mat kernel1 = getStructuringElement(MorphTypes::MORPH_ERODE, Size(3, 3));
	Mat kernel2 = getStructuringElement(MorphTypes::MORPH_DILATE, Size(3, 3));
	dilate(mask, mask, kernel2, Point(-1, -1), 1);
	erode(mask, mask, kernel1, Point(-1, -1), 1);

	erode(mask, mask, kernel1, Point(-1, -1), 1);
	dilate(mask, mask, kernel2, Point(-1, -1), 1);


	//erode(mask, mask, kernel1, Point(-1, -1), 3);

	/*
	// one needs these codes
	erode(mask, mask, kernel1, Point(-1, -1), 1);
	// one end
	*/

	double minCol, minRow, maxCol, maxRow;
	minMaxX(&mask, &minCol, &minRow, &maxCol, &maxRow);
	cout << "min row : " << minRow << ", col : " << minCol << endl;
	cout << "max row : " << maxRow << ", col : " << maxCol << endl;


	imshow("腐蚀膨胀后的mask图片", mask);

	Mat maskCopy = mask.clone();

	RotatedRect rect;


	findContours(mask, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);






	cout << "contours size : " << contours.size() << endl;
	cout << "dealing ..." << endl;

	vector<pair<int, double> > areaVector;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
		double area = contourArea(contours[contourIdx]);
		areaVector.push_back(make_pair(contourIdx, area));
	}
	sort(areaVector.begin(), areaVector.end(), [](pair<int, double> a, pair<int, double> b) {return (a.second>b.second);});

	vector<Point> needContour;

	needContour.insert(needContour.end(), contours[areaVector[0].first].begin(), contours[areaVector[0].first].end());
	//needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());

	cout << "max area : " << areaVector[0].second << endl;

	cout << "ci max area : " << areaVector[1].second << endl;
	if (contours.size() >= 2) {
		if (areaVector[1].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());
		}
	}
	else if (contours.size() >= 3) {
		if (areaVector[2].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[2].first].begin(), contours[areaVector[2].first].end());
		}
	}





	/*double maxArea = 0;
	int maxIdx = -1;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
	double area = contourArea(contours[contourIdx]);
	if (area > maxArea) {
	maxArea = area;
	maxIdx = contourIdx;
	}
	}

	rect = minAreaRect(contours[maxIdx]);*/

	rect = minAreaRect(needContour);

	Point2f vertices[4];
	rect.points(vertices);



	//////

	Point2f tempVertices[4];
	float verticesMeanX = 0;
	float verticesMeanY = 0;
	for (int i = 0; i < 4; ++i) {
		tempVertices[i] = vertices[i];
		verticesMeanX += vertices[i].x;
		verticesMeanY += vertices[i].y;
	}

	verticesMeanX = verticesMeanX / 4;
	verticesMeanY = verticesMeanY / 4;

	for (int i = 0; i < 4; ++i) {
		if (tempVertices[i].x<verticesMeanX && tempVertices[i].y>verticesMeanY) {
			vertices[0] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x < verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[1] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[2] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y > verticesMeanY) {
			vertices[3] = tempVertices[i];
			continue;
		}
	}

	//////






	for (int i = 0; i < 4; ++i) {
		cout << i << " : " << vertices[i].x << ", " << vertices[i].y << endl;
	}


	// five needs these codes
	Vec4f l1, l2, l3, l4;

	convertLinePoint2Vec(&vertices[0], &vertices[1], &l1);
	l1[2] = minCol;
	l1[3] = minRow;
	convertLinePoint2Vec(&vertices[1], &vertices[2], &l2);
	convertLinePoint2Vec(&vertices[2], &vertices[3], &l3);
	l3[2] = maxCol;
	l3[3] = maxRow;
	convertLinePoint2Vec(&vertices[3], &vertices[0], &l4);


	//move line

	imshow("result mask", mask);

	// l1
	int moveCountL1 = 0;
	while (!judgeLine(maskCopy, &l1, &l2, &l4, 255, 0.5) && moveCountL1 < 40) {
		moveLine(&l1, 1, 0);
		moveCountL1++;
	}

	// l2
	int moveCountL2 = 0;
	while (!judgeLine(maskCopy, &l2, &l1, &l3, 255, 0.6) && moveCountL2 < 40) {
		moveLine(&l2, 0, 1);
		moveCountL2++;
	}

	// l3
	int moveCountL3 = 0;
	while (!judgeLine(maskCopy, &l3, &l2, &l4, 255, 0.5) && moveCountL3 < 40) {
		moveLine(&l3, -1, 0);
		moveCountL3++;
	}

	//

	getCrossPoint(&l4, &l1, &vertices[0]);
	getCrossPoint(&l1, &l2, &vertices[1]);
	getCrossPoint(&l2, &l3, &vertices[2]);
	getCrossPoint(&l3, &l4, &vertices[3]);
	// five end


	for (int i = 0; i < 4; ++i) {
		line(dealImage1, vertices[i], vertices[(i + 1) % 4], Scalar(0));
		line(showImage, vertices[i], vertices[(i + 1) % 4], Scalar(0));
	}



	imshow("处理后的画图的图片", showImage);


	//imwrite(preSavePath + "dealImage1.bmp", dealImage1);
	//imwrite(preSavePath + "mask.bmp", mask);


	imshow("处理后的图片", dealImage1);
	cout << "dealImage1 : " << dealImage1.rows << "x" << dealImage1.cols << endl;


	imshow("处理后的Mask图片", mask);
	cout << "mask : " << mask.rows << "x" << mask.cols << endl;

	/*if (isSaveDealImage == true) {
		imwrite(preSavePath + "\\" + to_string(imgIndex) + "_" + to_string(imgCount) + ".bmp", showImage);
	}*/

	double resDist = -1;
	resDist = vertices[3].y - vertices[1].y;

	cout << "res dist : " << resDist << endl;
	return resDist;
}

double DetectEdges10::dealFiveDown() {
	// 这里依赖一个dealImage的Mat其他用到的都是全局变量，dealImage是传进来的
	// 基本上是ok的

	double areaSelectThreshold = 2000;


	uchar lut[256];
	copyUcharArr(lut5SDown, lut);

	/*for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut[i] = 255;
		}
		else if (i < 100) {
			lut[i] = 0;
		}
		else if (i < 150) {
			lut[i] = i - 50;
		}
		else if (i < 200) {
			lut[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut[i] = n;
			else
				lut[i] = n - 255;
		}
	}*/

	uchar lut1[256];
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut1[i] = 0;
		}
		else {
			lut1[i] = 255;
		}
	}


	dealImage = dealImage(dealBox);
	dealImage.convertTo(dealImage, CV_8UC1);
	cout << "hello world" << endl;
	Mat showImage;
	showImage = dealImage.clone();

	//imwrite(preSavePath + "save.bmp", dealImage);
	cout << "save image : " << dealImage.rows << "x" << dealImage.cols << endl;


	imshow("处理前的图片", dealImage);

	uchar *p;
	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage.ptr<uchar>(r);
		for (int c = 0; c < dealImage.cols; ++c) {
			p[c] = lut[p[c]];
		}
	}


	imshow("处理的图片", dealImage);


	dealImage1 = dealImage.clone();
	//threshold(dealImage1, dealImage1, 230, 255, ThresholdTypes::THRESH_BINARY);

	//blur(dealImage1, dealImage1, Size(3, 3));
	//Mat grad1;
	//Mat grad2;
	//Sobel(dealImage1, grad1, CV_8U, 1, 0);
	//Sobel(dealImage1, grad2, CV_8U, 0, 1);
	//grad1 = cv::abs(grad1 - grad2);
	//dealImage1 = grad1;

	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage1.ptr<uchar>(r);
		for (int c = 0; c < dealImage1.cols; ++c) {
			p[c] = lut1[p[c]];
		}
	}

	imshow("处理的图片", dealImage1);

	Mat mask = dealImage1 > 100;

	imshow("刚得到的mask图片", mask);


	Mat imageCopy;
	dealImage1.copyTo(imageCopy, mask);

	Mat kernel1 = getStructuringElement(MorphTypes::MORPH_ERODE, Size(3, 3));
	Mat kernel2 = getStructuringElement(MorphTypes::MORPH_DILATE, Size(3, 3));
	dilate(mask, mask, kernel2, Point(-1, -1), 1);
	erode(mask, mask, kernel1, Point(-1, -1), 1);

	erode(mask, mask, kernel1, Point(-1, -1), 1);
	dilate(mask, mask, kernel2, Point(-1, -1), 1);


	//erode(mask, mask, kernel1, Point(-1, -1), 3);

	/*
	// one needs these codes
	erode(mask, mask, kernel1, Point(-1, -1), 1);
	// one end
	*/

	double minCol, minRow, maxCol, maxRow;
	minMaxX(&mask, &minCol, &minRow, &maxCol, &maxRow);
	cout << "min row : " << minRow << ", col : " << minCol << endl;
	cout << "max row : " << maxRow << ", col : " << maxCol << endl;


	imshow("腐蚀膨胀后的mask图片", mask);

	Mat maskCopy = mask.clone();

	RotatedRect rect;


	findContours(mask, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);






	cout << "contours size : " << contours.size() << endl;
	cout << "dealing ..." << endl;

	vector<pair<int, double> > areaVector;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
		double area = contourArea(contours[contourIdx]);
		areaVector.push_back(make_pair(contourIdx, area));
	}
	sort(areaVector.begin(), areaVector.end(), [](pair<int, double> a, pair<int, double> b) {return (a.second>b.second);});

	vector<Point> needContour;

	needContour.insert(needContour.end(), contours[areaVector[0].first].begin(), contours[areaVector[0].first].end());
	//needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());

	cout << "max area : " << areaVector[0].second << endl;

	cout << "ci max area : " << areaVector[1].second << endl;
	if (contours.size() >= 2) {
		if (areaVector[1].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());
		}
	}
	else if (contours.size() >= 3) {
		if (areaVector[2].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[2].first].begin(), contours[areaVector[2].first].end());
		}
	}





	/*double maxArea = 0;
	int maxIdx = -1;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
	double area = contourArea(contours[contourIdx]);
	if (area > maxArea) {
	maxArea = area;
	maxIdx = contourIdx;
	}
	}

	rect = minAreaRect(contours[maxIdx]);*/

	rect = minAreaRect(needContour);

	Point2f vertices[4];
	rect.points(vertices);



	//////

	Point2f tempVertices[4];
	float verticesMeanX = 0;
	float verticesMeanY = 0;
	for (int i = 0; i < 4; ++i) {
		tempVertices[i] = vertices[i];
		verticesMeanX += vertices[i].x;
		verticesMeanY += vertices[i].y;
	}

	verticesMeanX = verticesMeanX / 4;
	verticesMeanY = verticesMeanY / 4;

	for (int i = 0; i < 4; ++i) {
		if (tempVertices[i].x<verticesMeanX && tempVertices[i].y>verticesMeanY) {
			vertices[0] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x < verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[1] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[2] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y > verticesMeanY) {
			vertices[3] = tempVertices[i];
			continue;
		}
	}

	//////






	for (int i = 0; i < 4; ++i) {
		cout << i << " : " << vertices[i].x << ", " << vertices[i].y << endl;
	}


	// five needs these codes
	Vec4f l1, l2, l3, l4;

	convertLinePoint2Vec(&vertices[0], &vertices[1], &l1);
	l1[2] = minCol;
	l1[3] = minRow;
	convertLinePoint2Vec(&vertices[1], &vertices[2], &l2);
	convertLinePoint2Vec(&vertices[2], &vertices[3], &l3);
	l3[2] = maxCol;
	l3[3] = maxRow;
	convertLinePoint2Vec(&vertices[3], &vertices[0], &l4);


	//move line

	imshow("result mask", mask);

	// l1
	int moveCountL1 = 0;
	while (!judgeLine(maskCopy, &l1, &l2, &l4, 255, 0.5) && moveCountL1 < 40) {
		moveLine(&l1, 1, 0);
		moveCountL1++;
	}

	// l3
	int moveCountL3 = 0;
	while (!judgeLine(maskCopy, &l3, &l2, &l4, 255, 0.5) && moveCountL3 < 40) {
		moveLine(&l3, -1, 0);
		moveCountL3++;
	}

	// l4
	int moveCountL4 = 0;
	while (!judgeLine(maskCopy, &l4, &l1, &l3, 255, 0.8) && moveCountL4 < 40) {
		moveLine(&l4, 0, -1);
		moveCountL4++;
	}


	//

	getCrossPoint(&l4, &l1, &vertices[0]);
	getCrossPoint(&l1, &l2, &vertices[1]);
	getCrossPoint(&l2, &l3, &vertices[2]);
	getCrossPoint(&l3, &l4, &vertices[3]);
	// five end


	for (int i = 0; i < 4; ++i) {
		line(dealImage1, vertices[i], vertices[(i + 1) % 4], Scalar(0));
		line(showImage, vertices[i], vertices[(i + 1) % 4], Scalar(0));
	}



	imshow("处理后的画图的图片", showImage);


	//imwrite(preSavePath + "dealImage1.bmp", dealImage1);
	//imwrite(preSavePath + "mask.bmp", mask);


	imshow("处理后的图片", dealImage1);
	cout << "dealImage1 : " << dealImage1.rows << "x" << dealImage1.cols << endl;


	imshow("处理后的Mask图片", mask);
	cout << "mask : " << mask.rows << "x" << mask.cols << endl;

	/*if (isSaveDealImage == true) {
		imwrite(preSavePath + "\\" + to_string(imgIndex) + "_" + to_string(imgCount) + ".bmp", showImage);
	}*/

	double resDist = -1;
	resDist = vertices[3].y - vertices[1].y;

	cout << "res dist : " << resDist << endl;

	return resDist;
}

void DetectEdges10::dealFiveLeft() {
	// 这里依赖一个dealImage的Mat其他用到的都是全局变量，dealImage是传进来的
	// 基本上是ok的

	double moveDist = 92;
	double areaSelectThreshold = 2000;


	uchar lut[256];
	copyUcharArr(lut5SLeft, lut);

	/*for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut[i] = 255;
		}
		else if (i < 100) {
			lut[i] = 0;
		}
		else if (i < 150) {
			lut[i] = i - 50;
		}
		else if (i < 200) {
			lut[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut[i] = n;
			else
				lut[i] = n - 255;
		}
	}*/

	uchar lut1[256];
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut1[i] = 0;
		}
		else {
			lut1[i] = 255;
		}
	}


	dealImage = dealImage(dealBox);
	dealImage.convertTo(dealImage, CV_8UC1);
	cout << "hello world" << endl;
	Mat showImage;
	showImage = dealImage.clone();

	//imwrite(preSavePath + "save.bmp", dealImage);
	cout << "save image : " << dealImage.rows << "x" << dealImage.cols << endl;


	imshow("处理前的图片", dealImage);

	uchar *p;
	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage.ptr<uchar>(r);
		for (int c = 0; c < dealImage.cols; ++c) {
			p[c] = lut[p[c]];
		}
	}


	imshow("处理的图片", dealImage);


	dealImage1 = dealImage.clone();
	//threshold(dealImage1, dealImage1, 230, 255, ThresholdTypes::THRESH_BINARY);

	//blur(dealImage1, dealImage1, Size(3, 3));
	//Mat grad1;
	//Mat grad2;
	//Sobel(dealImage1, grad1, CV_8U, 1, 0);
	//Sobel(dealImage1, grad2, CV_8U, 0, 1);
	//grad1 = cv::abs(grad1 - grad2);
	//dealImage1 = grad1;

	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage1.ptr<uchar>(r);
		for (int c = 0; c < dealImage1.cols; ++c) {
			p[c] = lut1[p[c]];
		}
	}

	imshow("处理的图片", dealImage1);

	Mat mask = dealImage1 > 100;

	imshow("刚得到的mask图片", mask);


	Mat imageCopy;
	dealImage1.copyTo(imageCopy, mask);

	Mat kernel1 = getStructuringElement(MorphTypes::MORPH_ERODE, Size(3, 3));
	Mat kernel2 = getStructuringElement(MorphTypes::MORPH_DILATE, Size(3, 3));
	dilate(mask, mask, kernel2, Point(-1, -1), 1);
	erode(mask, mask, kernel1, Point(-1, -1), 1);

	erode(mask, mask, kernel1, Point(-1, -1), 1);
	dilate(mask, mask, kernel2, Point(-1, -1), 1);


	//erode(mask, mask, kernel1, Point(-1, -1), 3);

	/*
	// one needs these codes
	erode(mask, mask, kernel1, Point(-1, -1), 1);
	// one end
	*/

	double minCol, minRow, maxCol, maxRow;
	minMaxX(&mask, &minCol, &minRow, &maxCol, &maxRow);
	cout << "min row : " << minRow << ", col : " << minCol << endl;
	cout << "max row : " << maxRow << ", col : " << maxCol << endl;


	imshow("腐蚀膨胀后的mask图片", mask);

	Mat maskCopy = mask.clone();

	RotatedRect rect;


	findContours(mask, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);






	cout << "contours size : " << contours.size() << endl;
	cout << "dealing ..." << endl;

	vector<pair<int, double> > areaVector;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
		double area = contourArea(contours[contourIdx]);
		areaVector.push_back(make_pair(contourIdx, area));
	}
	sort(areaVector.begin(), areaVector.end(), [](pair<int, double> a, pair<int, double> b) {return (a.second>b.second);});

	vector<Point> needContour;

	needContour.insert(needContour.end(), contours[areaVector[0].first].begin(), contours[areaVector[0].first].end());
	//needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());

	cout << "max area : " << areaVector[0].second << endl;

	cout << "ci max area : " << areaVector[1].second << endl;
	if (contours.size() >= 2) {
		if (areaVector[1].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());
		}
	}
	else if (contours.size() >= 3) {
		if (areaVector[2].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[2].first].begin(), contours[areaVector[2].first].end());
		}
	}





	/*double maxArea = 0;
	int maxIdx = -1;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
	double area = contourArea(contours[contourIdx]);
	if (area > maxArea) {
	maxArea = area;
	maxIdx = contourIdx;
	}
	}

	rect = minAreaRect(contours[maxIdx]);*/

	rect = minAreaRect(needContour);

	Point2f vertices[4];
	rect.points(vertices);



	//////

	Point2f tempVertices[4];
	float verticesMeanX = 0;
	float verticesMeanY = 0;
	for (int i = 0; i < 4; ++i) {
		tempVertices[i] = vertices[i];
		verticesMeanX += vertices[i].x;
		verticesMeanY += vertices[i].y;
	}

	verticesMeanX = verticesMeanX / 4;
	verticesMeanY = verticesMeanY / 4;

	for (int i = 0; i < 4; ++i) {
		if (tempVertices[i].x<verticesMeanX && tempVertices[i].y>verticesMeanY) {
			vertices[0] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x < verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[1] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[2] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y > verticesMeanY) {
			vertices[3] = tempVertices[i];
			continue;
		}
	}

	//////






	for (int i = 0; i < 4; ++i) {
		cout << i << " : " << vertices[i].x << ", " << vertices[i].y << endl;
	}


	// five needs these codes
	Vec4f l1, l2, l3, l4;

	convertLinePoint2Vec(&vertices[0], &vertices[1], &l1);
	l1[2] = minCol;
	l1[3] = minRow;
	convertLinePoint2Vec(&vertices[1], &vertices[2], &l2);
	//convertLinePoint2Vec(&vertices[2], &vertices[3], &l3);
	//l3[2] = maxCol;
	//l3[3] = maxRow;
	l3 = l1;
	moveLine(&l3, moveDist, 0);
	convertLinePoint2Vec(&vertices[3], &vertices[0], &l4);


	//move line

	imshow("result mask", mask);

	// l1
	int moveCountL1 = 0;
	while (!judgeLine(maskCopy, &l1, &l2, &l4, 255, 0.1) && moveCountL1 < 40) {
		moveLine(&l1, 1, 0);
		moveCountL1++;
	}

	// l2
	int moveCountL2 = 0;
	while (!judgeLine(maskCopy, &l2, &l1, &l3, 255, 0.5) && moveCountL2 < 40) {
		moveLine(&l2, 0, 1);
		moveCountL2++;
	}

	// l4
	int moveCountL4 = 0;
	while (!judgeLine(maskCopy, &l4, &l1, &l3, 255, 0.8) && moveCountL4 < 40) {
		moveLine(&l4, 0, -1);
		moveCountL4++;
	}

	//l3
	l3 = l1;
	moveLine(&l3, moveDist, 0);


	//

	getCrossPoint(&l4, &l1, &vertices[0]);
	getCrossPoint(&l1, &l2, &vertices[1]);
	getCrossPoint(&l2, &l3, &vertices[2]);
	getCrossPoint(&l3, &l4, &vertices[3]);
	// five end


	for (int i = 0; i < 4; ++i) {
		line(dealImage1, vertices[i], vertices[(i + 1) % 4], Scalar(0));
		line(showImage, vertices[i], vertices[(i + 1) % 4], Scalar(0));
	}



	imshow("处理后的画图的图片", showImage);


	//imwrite(preSavePath + "dealImage1.bmp", dealImage1);
	//imwrite(preSavePath + "mask.bmp", mask);


	imshow("处理后的图片", dealImage1);
	cout << "dealImage1 : " << dealImage1.rows << "x" << dealImage1.cols << endl;


	imshow("处理后的Mask图片", mask);
	cout << "mask : " << mask.rows << "x" << mask.cols << endl;

	/*if (isSaveDealImage == true) {
		imwrite(preSavePath + "\\" + to_string(imgIndex) + "_" + to_string(imgCount) + ".bmp", showImage);
	}*/


}

void DetectEdges10::dealFiveRight() {
	// 这里依赖一个dealImage的Mat其他用到的都是全局变量，dealImage是传进来的
	// 基本上是ok的

	double moveDist = 92;
	double areaSelectThreshold = 2000;


	uchar lut[256];
	copyUcharArr(lut5SRight, lut);

	/*for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut[i] = 255;
		}
		else if (i < 100) {
			lut[i] = 0;
		}
		else if (i < 150) {
			lut[i] = i - 50;
		}
		else if (i < 200) {
			lut[i] = pow(abs(i - 180) / 50, 2) * 50 + 200;
		}
		else {
			int n = i + 50;
			if (n < 255)
				lut[i] = n;
			else
				lut[i] = n - 255;
		}
	}*/

	uchar lut1[256];
	for (int i = 0; i < 256; ++i) {
		if (i < 50) {
			lut1[i] = 0;
		}
		else {
			lut1[i] = 255;
		}
	}


	dealImage = dealImage(dealBox);
	dealImage.convertTo(dealImage, CV_8UC1);
	cout << "hello world" << endl;
	Mat showImage;
	showImage = dealImage.clone();

	//imwrite(preSavePath + "save.bmp", dealImage);
	cout << "save image : " << dealImage.rows << "x" << dealImage.cols << endl;


	imshow("处理前的图片", dealImage);

	uchar *p;
	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage.ptr<uchar>(r);
		for (int c = 0; c < dealImage.cols; ++c) {
			p[c] = lut[p[c]];
		}
	}


	imshow("处理的图片", dealImage);


	dealImage1 = dealImage.clone();
	//threshold(dealImage1, dealImage1, 230, 255, ThresholdTypes::THRESH_BINARY);

	//blur(dealImage1, dealImage1, Size(3, 3));
	//Mat grad1;
	//Mat grad2;
	//Sobel(dealImage1, grad1, CV_8U, 1, 0);
	//Sobel(dealImage1, grad2, CV_8U, 0, 1);
	//grad1 = cv::abs(grad1 - grad2);
	//dealImage1 = grad1;

	for (int r = 0; r < dealImage.rows; ++r) {
		p = dealImage1.ptr<uchar>(r);
		for (int c = 0; c < dealImage1.cols; ++c) {
			p[c] = lut1[p[c]];
		}
	}

	imshow("处理的图片", dealImage1);

	Mat mask = dealImage1 > 100;

	imshow("刚得到的mask图片", mask);


	Mat imageCopy;
	dealImage1.copyTo(imageCopy, mask);

	Mat kernel1 = getStructuringElement(MorphTypes::MORPH_ERODE, Size(3, 3));
	Mat kernel2 = getStructuringElement(MorphTypes::MORPH_DILATE, Size(3, 3));
	dilate(mask, mask, kernel2, Point(-1, -1), 1);
	erode(mask, mask, kernel1, Point(-1, -1), 1);

	erode(mask, mask, kernel1, Point(-1, -1), 1);
	dilate(mask, mask, kernel2, Point(-1, -1), 1);


	//erode(mask, mask, kernel1, Point(-1, -1), 3);

	/*
	// one needs these codes
	erode(mask, mask, kernel1, Point(-1, -1), 1);
	// one end
	*/

	double minCol, minRow, maxCol, maxRow;
	minMaxX(&mask, &minCol, &minRow, &maxCol, &maxRow);
	cout << "min row : " << minRow << ", col : " << minCol << endl;
	cout << "max row : " << maxRow << ", col : " << maxCol << endl;


	imshow("腐蚀膨胀后的mask图片", mask);

	Mat maskCopy = mask.clone();

	RotatedRect rect;


	findContours(mask, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);






	cout << "contours size : " << contours.size() << endl;
	cout << "dealing ..." << endl;

	vector<pair<int, double> > areaVector;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
		double area = contourArea(contours[contourIdx]);
		areaVector.push_back(make_pair(contourIdx, area));
	}
	sort(areaVector.begin(), areaVector.end(), [](pair<int, double> a, pair<int, double> b) {return (a.second>b.second);});

	vector<Point> needContour;

	needContour.insert(needContour.end(), contours[areaVector[0].first].begin(), contours[areaVector[0].first].end());
	//needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());

	cout << "max area : " << areaVector[0].second << endl;


	if (contours.size() >= 2) {
		cout << "ci max area : " << areaVector[1].second << endl;
		if (areaVector[1].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[1].first].begin(), contours[areaVector[1].first].end());
		}
	}
	else if (contours.size() >= 3) {
		if (areaVector[2].second > areaSelectThreshold) {
			needContour.insert(needContour.end(), contours[areaVector[2].first].begin(), contours[areaVector[2].first].end());
		}
	}





	/*double maxArea = 0;
	int maxIdx = -1;
	for (int contourIdx = 0; contourIdx < contours.size(); ++contourIdx) {
	double area = contourArea(contours[contourIdx]);
	if (area > maxArea) {
	maxArea = area;
	maxIdx = contourIdx;
	}
	}

	rect = minAreaRect(contours[maxIdx]);*/

	rect = minAreaRect(needContour);

	Point2f vertices[4];
	rect.points(vertices);



	//////

	Point2f tempVertices[4];
	float verticesMeanX = 0;
	float verticesMeanY = 0;
	for (int i = 0; i < 4; ++i) {
		tempVertices[i] = vertices[i];
		verticesMeanX += vertices[i].x;
		verticesMeanY += vertices[i].y;
	}

	verticesMeanX = verticesMeanX / 4;
	verticesMeanY = verticesMeanY / 4;

	for (int i = 0; i < 4; ++i) {
		if (tempVertices[i].x<verticesMeanX && tempVertices[i].y>verticesMeanY) {
			vertices[0] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x < verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[1] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[2] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y > verticesMeanY) {
			vertices[3] = tempVertices[i];
			continue;
		}
	}

	//////






	for (int i = 0; i < 4; ++i) {
		cout << i << " : " << vertices[i].x << ", " << vertices[i].y << endl;
	}


	// five needs these codes
	Vec4f l1, l2, l3, l4;

	convertLinePoint2Vec(&vertices[0], &vertices[1], &l1);
	//l1[2] = minCol;
	//l1[3] = minRow;
	convertLinePoint2Vec(&vertices[1], &vertices[2], &l2);
	convertLinePoint2Vec(&vertices[2], &vertices[3], &l3);
	l3[2] = maxCol;
	l3[3] = maxRow;
	convertLinePoint2Vec(&vertices[3], &vertices[0], &l4);
	l1 = l3;
	moveLine(&l1, -moveDist, 0);


	//move line

	imshow("result mask", mask);

	// l3
	int moveCountL3 = 0;
	while (!judgeLine(maskCopy, &l3, &l2, &l4, 255, 0.2) && moveCountL3 < 40) {
		moveLine(&l3, -1, 0);
		moveCountL3++;
	}

	// l2
	int moveCountL2 = 0;
	while (!judgeLine(maskCopy, &l2, &l1, &l3, 255, 0.5) && moveCountL2 < 40) {
		moveLine(&l2, 0, 1);
		moveCountL2++;
	}

	// l4
	int moveCountL4 = 0;
	while (!judgeLine(maskCopy, &l4, &l1, &l3, 255, 0.8) && moveCountL4 < 40) {
		moveLine(&l4, 0, -1);
		moveCountL4++;
	}

	// l1
	l1 = l3;
	moveLine(&l1, -moveDist, 0);


	//

	getCrossPoint(&l4, &l1, &vertices[0]);
	getCrossPoint(&l1, &l2, &vertices[1]);
	getCrossPoint(&l2, &l3, &vertices[2]);
	getCrossPoint(&l3, &l4, &vertices[3]);
	// five end


	for (int i = 0; i < 4; ++i) {
		line(dealImage1, vertices[i], vertices[(i + 1) % 4], Scalar(0));
		line(showImage, vertices[i], vertices[(i + 1) % 4], Scalar(0));
	}



	imshow("处理后的画图的图片", showImage);


	//imwrite(preSavePath + "dealImage1.bmp", dealImage1);
	//imwrite(preSavePath + "mask.bmp", mask);


	imshow("处理后的图片", dealImage1);
	cout << "dealImage1 : " << dealImage1.rows << "x" << dealImage1.cols << endl;


	imshow("处理后的Mask图片", mask);
	cout << "mask : " << mask.rows << "x" << mask.cols << endl;

	/*if (isSaveDealImage == true) {
		imwrite(preSavePath + "\\" + to_string(imgIndex) + "_" + to_string(imgCount) + ".bmp", showImage);
	}*/


}

/*********************** detect edges end *************************/

/*********************** utilities ********************************/
void DetectEdges10::minMaxX(Mat *m, double *minX, double *minY, double *maxX, double *maxY) {
	bool found = false;
	uchar *pm;
	for (int c = 0; c < m->cols; ++c) {
		for (int r = 0; r < m->rows; ++r) {
			pm = m->ptr<uchar>(r);
			if (pm[c] == 255) {
				found = true;
				*minX = c;
				*minY = r;
				break;
			}
		}
		if (found == true)
			break;
	}
	found = false;
	for (int c = m->cols - 1; c >= 0; --c) {
		for (int r = 0; r < m->rows; ++r) {
			pm = m->ptr<uchar>(r);
			if (pm[c] == 255) {
				found = true;
				*maxX = c;
				*maxY = r;
				break;
			}
		}
		if (found == true)
			break;
	}
}

void DetectEdges10::getCrossPoint(Vec4f *l1, Vec4f *l2, Point2f *p) {
	double vx1 = (*l1)[0];
	double vy1 = (*l1)[1];
	double x1 = (*l1)[2];
	double y1 = (*l1)[3];

	double vx2 = (*l2)[0];
	double vy2 = (*l2)[1];
	double x2 = (*l2)[2];
	double y2 = (*l2)[3];

	double x = (x2*vx1*vy2 - y2*vx1*vx2 - x1*vx2*vy1 + y1*vx1*vx2) / (vx1*vy2 - vx2*vy1);
	double y = (x2*vy1*vy2 - y2*vx2*vy1 - x1*vy1*vy2 + y1*vx1*vy2) / (vx1*vy2 - vx2*vy1);

	p->x = x;
	p->y = y;
}

void DetectEdges10::convertLinePoint2Vec(Point2f *p1, Point2f *p2, Vec4f *l) {
	double p1x = p1->x;
	double p1y = p1->y;
	double p2x = p2->x;
	double p2y = p2->y;

	double vx = p2x - p1x;
	double vy = p2y - p1y;

	(*l)[2] = p1x;
	(*l)[3] = p1y;

	(*l)[0] = vx / sqrt(vx*vx + vy*vy);
	(*l)[1] = vy / sqrt(vx*vx + vy*vy);
}

void DetectEdges10::moveLine(Vec4f *l, float distX, float distY) {
	(*l)[2] = (*l)[2] + distX;
	(*l)[3] = (*l)[3] + distY;
}

bool DetectEdges10::equalPointPixel(Mat img, Point2f *p, uchar pixel) {
	int pointx = (*p).x;
	int pointy = (*p).y;
	int pixelStep = 10;
	uchar pixel00 = img.at<uchar>(pointy - pixelStep, pointx - pixelStep);
	uchar pixel01 = img.at<uchar>(pointy, pointx - pixelStep);
	uchar pixel02 = img.at<uchar>(pointy + pixelStep, pointx - pixelStep);
	pixel00 = 0; pixel01 = 0; pixel02 = 0;
	uchar pixel10 = img.at<uchar>(pointy - pixelStep, pointx);
	uchar pixel11 = img.at<uchar>(pointy, pointx);
	uchar pixel12 = img.at<uchar>(pointy + pixelStep, pointx);
	uchar pixel20 = img.at<uchar>(pointy - pixelStep, pointx + pixelStep);
	uchar pixel21 = img.at<uchar>(pointy, pointx + pixelStep);
	uchar pixel22 = img.at<uchar>(pointy + pixelStep, pointx + pixelStep);
	pixel20 = 0; pixel21 = 0; pixel22 = 0;

	if (pixel00 == pixel || pixel01 == pixel || pixel02 == pixel ||
		pixel10 == pixel || pixel11 == pixel || pixel12 == pixel ||
		pixel20 == pixel || pixel21 == pixel || pixel22 == pixel)
		return true;

	return false;
}

vector<Point2f> DetectEdges10::getLineSegmentPoint(Point2f *p1, Point2f *p2) {
	double xmin, xmax, ymin, ymax;
	if (p1->x < p2->x) {
		xmin = int(p1->x) + 1;
		xmax = int(p2->x);
	}
	else {
		xmin = int(p2->x) + 1;
		xmax = int(p1->x);
	}

	if (p1->y < p2->y) {
		ymin = int(p1->y) + 1;
		ymax = int(p2->y);
	}
	else {
		ymin = int(p2->y) + 1;
		ymax = int(p1->y);
	}

	vector<Point2f> res;
	// p1 -> p2
	double p1x = p1->x;
	double p1y = p1->y;
	double p2x = p2->x;
	double p2y = p2->y;

	double vx = p2x - p1x;
	double vy = p2y - p1y;

	double distP12 = sqrt(vx*vx + vy*vy);

	double pvx = vx / distP12;
	double pvy = vy / distP12;

	int count = 0;

	double px = p1x + count*pvx;
	double py = p1y + count*pvy;

	while (sqrt(pow(px - p1x, 2) + pow(py - p1y, 2))<distP12) {
		if (px < xmin) px = xmin;
		if (px > xmax) px = xmax;
		if (py < ymin) py = ymin;
		if (py > ymax) py = ymax;

		res.push_back(Point2f(px, py));
		count++;
		px = p1x + count*pvx;
		py = p1y + count*pvy;
	}
	return res;
}

bool DetectEdges10::judgeLine(Mat img, Vec4f *l, Vec4f *l1, Vec4f *l2, uchar pixel, float percent, int thres = 2) {
	int test = pixel;
	Point2f p1;
	Point2f p2;
	getCrossPoint(l, l1, &p1);
	getCrossPoint(l, l2, &p2);
	vector<Point2f> pointSet = getLineSegmentPoint(&p1, &p2);

	//cout << "pointSet size : " << pointSet.size() << endl;

	int count = 0;
	for (int i = 0; i < pointSet.size(); ++i) {
		int px = pointSet[i].x;
		int py = pointSet[i].y;
		int test1;

		try {
			test1 = img.at<uchar>(py, px);
		}
		catch (exception &e) {
			continue;
		}

		if (abs(test - test1) < thres) {
			count++;
		}
	}

	float curPercent = ((float)count) / ((float)pointSet.size());

	//cout << "curPercent : " << curPercent << endl;

	if (curPercent > percent)
		return true;
	else
		return false;
}


// 2018.12.07  adding
vector<uchar> DetectEdges10::filterPointSetPixel(vector<uchar> &pv, double percent) {
	sort(pv.begin(), pv.end());
	int originSize = pv.size();
	int n = originSize * (1 - percent) / 2;
	int nEnd = originSize - n;
	vector<uchar> res;
	for (int i = n; i < nEnd; ++i) {
		res.push_back(pv[i]);
	}
	return res;
}

// 2018.12.06  adding
void DetectEdges10::calcPixelStatistic(vector<uchar> &pv, double *meanPixel, double *stddev, double *maxPixel, double *minPixel) {
	double s = 0;
	(*maxPixel) = 0;
	(*minPixel) = 255;
	for (int i = 0; i < pv.size(); ++i) {
		s += pv[i];
		if (pv[i] >(*maxPixel))
			(*maxPixel) = pv[i];
		if (pv[i] < (*minPixel))
			(*minPixel) = pv[i];
	}
	(*meanPixel) = s / pv.size();

	s = 0;
	for (int i = 0; i < pv.size(); ++i) {
		s += pow(pv[i] - (*meanPixel), 2);
	}
	(*stddev) = sqrt(s / pv.size());
}

// 2018.12.06  adding
bool DetectEdges10::judgeLinePixelStatistic(Mat img, Vec4f *l, Vec4f *l1, Vec4f *l2, double percent, double stddev, double thres, double maxMinPixelDist = -1) {
	Point2f p1;
	Point2f p2;
	getCrossPoint(l, l1, &p1);
	getCrossPoint(l, l2, &p2);
	vector<Point2f> pointSet = getLineSegmentPoint(&p1, &p2);

	vector<uchar> pointPixelSet;

	int count = 0;
	for (int i = 0; i < pointSet.size(); ++i) {
		int px = pointSet[i].x;
		int py = pointSet[i].y;

		uchar test1 = img.at<uchar>(py, px);

		pointPixelSet.push_back(test1);
	}

	pointPixelSet = filterPointSetPixel(pointPixelSet, percent);

	double pMeanPixel, pMaxPixel, pMinPixel, pStddev;
	calcPixelStatistic(pointPixelSet, &pMeanPixel, &pStddev, &pMaxPixel, &pMinPixel);

	bool judgeStddevFlag = false;
	bool judgeMaxMinPixelFlag = false;

	if (stddev == -1) {
		judgeStddevFlag = false;
	}
	else if (abs(pStddev - stddev) > thres) {
		judgeStddevFlag = false;
	}
	else {
		judgeStddevFlag = true;
	}


	if (maxMinPixelDist == -1) {
		judgeMaxMinPixelFlag = false;
	}
	else if (abs(pMaxPixel - pMinPixel) > maxMinPixelDist) {
		judgeMaxMinPixelFlag = false;
	}
	else {
		judgeMaxMinPixelFlag = true;
	}

	if (judgeStddevFlag || judgeMaxMinPixelFlag) {
		return true;
	}
	else {
		return false;
	}

	/*if (abs(pStddev - stddev) > thres) {
	return false;
	}
	else {
	if (maxMinPixelDist != -1) {
	if (abs(pMaxPixel - pMinPixel) > maxMinPixelDist) {
	return false;
	}
	}

	return true;
	}*/

}

// 2018.12.06  adding
bool DetectEdges10::judgeLinePixelGrad(Mat img, Vec4f *l, Vec4f *l1, Vec4f *l2, double thres, int oriention, int step, double percent, int selec = 0) {
	// oriention : 1 -> left 2 right
	//             2 -> right 2 left
	//             3 -> down 2 up
	//             4 -> up 2 down
	Point2f p1;
	Point2f p2;
	getCrossPoint(l, l1, &p1);
	getCrossPoint(l, l2, &p2);
	vector<Point2f> pointSet = getLineSegmentPoint(&p1, &p2);
	int count = 0;

	for (int i = 0; i < pointSet.size(); ++i) {
		int px = pointSet[i].x;
		int py = pointSet[i].y;

		uchar test1, test2;

		switch (oriention) {
		case 1:
			try {
				if (selec == 0)  test1 = img.at<uchar>(py, px);
				else  test1 = img.at<uchar>(py, px - step);
				test2 = img.at<uchar>(py, px + step);
			}
			catch (exception &e) {
				continue;
			}

			break;
		case 2:
			try {
				if (selec == 0)  test1 = img.at<uchar>(py, px);
				else  test1 = img.at<uchar>(py, px + step);
				test2 = img.at<uchar>(py, px - step);
			}
			catch (exception &e) {
				continue;
			}

			break;
		case 3:
			try {
				if (selec == 0)  test1 = img.at<uchar>(py, px);
				else  test1 = img.at<uchar>(py + step, px);
				test2 = img.at<uchar>(py - step, px);
			}
			catch (exception &e) {
				continue;
			}
			break;
		case 4:
			try {
				if (selec == 0)  test1 = img.at<uchar>(py, px);
				else  test1 = img.at<uchar>(py - step, px);
				test2 = img.at<uchar>(py + step, px);
			}
			catch (exception &e) {
				continue;
			}
			break;
		default:
			return false;
		}

		if (abs(test1 - test2) < thres)
			count++;
	}

	double curPercent = (double)count / pointSet.size();

	if (curPercent > percent)
		return true;
	else
		return false;
}

// 2018.12.07  adding
Vec4f DetectEdges10::selectLineFromRegionGrad(Mat img, Vec4f *l1, Vec4f *l2, Vec4f *l3, Vec4f *l4, double thres, int oriention, double percent, bool PN) {
	// oriention : 1 -> left 2 right
	//             2 -> right 2 left
	//             3 -> down 2 up
	//             4 -> up 2 down
	Point2f pbegin1, pbegin2, pend1, pend2;
	Vec4f lbegin, lend, lborder1, lborder2, tempL;
	switch (oriention) {
	case 1:
		lbegin = (*l1);
		lend = (*l3);
		lborder1 = (*l2);
		lborder2 = (*l4);
		break;
	case 2:
		lbegin = (*l3);
		lend = (*l1);
		lborder1 = (*l2);
		lborder2 = (*l4);
		break;
	case 3:
		lbegin = (*l4);
		lend = (*l2);
		lborder1 = (*l1);
		lborder2 = (*l3);
		break;
	case 4:
		lbegin = (*l2);
		lend = (*l4);
		lborder1 = (*l1);
		lborder2 = (*l3);
		break;
	default:
		return Vec4f(-1, -1, -1, -1);
	}

	tempL = lbegin;

	Point2f tempP1, tempP2;
	Point2f centerPBegin, centerPEnd;
	getCrossPoint(&lbegin, &lborder1, &tempP1);
	getCrossPoint(&lbegin, &lborder2, &tempP2);
	centerPBegin.x = (tempP1.x + tempP2.x) / 2;
	centerPBegin.y = (tempP1.y + tempP2.y) / 2;
	getCrossPoint(&lend, &lborder1, &tempP1);
	getCrossPoint(&lend, &lborder2, &tempP2);
	centerPEnd.x = (tempP1.x + tempP2.x) / 2;
	centerPEnd.y = (tempP1.y + tempP2.y) / 2;

	bool positionFlag = true;
	bool judgeFlag = true;

	switch (oriention) {
	case 1:
		positionFlag = centerPBegin.x < centerPEnd.x;
		break;
	case 2:
		positionFlag = centerPBegin.x > centerPEnd.x;
		break;
	case 3:
		positionFlag = centerPBegin.y > centerPEnd.y;
		break;
	case 4:
		positionFlag = centerPBegin.y < centerPEnd.y;
		break;
	default:
		return Vec4f(-1, -1, -1, -1);
	}

	judgeFlag = judgeLinePixelGrad(img, &lbegin, &lborder1, &lborder2, thres, oriention, 2, percent);

	if (!PN)  judgeFlag = !judgeFlag;

	while (positionFlag && judgeFlag) {
		switch (oriention) {
		case 1:
			moveLine(&lbegin, 1, 0);
			break;
		case 2:
			moveLine(&lbegin, -1, 0);
			break;
		case 3:
			moveLine(&lbegin, 0, -1);
			break;
		case 4:
			moveLine(&lbegin, 0, 1);
			break;
		default:
			return Vec4f(-1, -1, -1, -1);
		}


		getCrossPoint(&lbegin, &lborder1, &tempP1);
		getCrossPoint(&lbegin, &lborder2, &tempP2);
		centerPBegin.x = (tempP1.x + tempP2.x) / 2;
		centerPBegin.y = (tempP1.y + tempP2.y) / 2;
		getCrossPoint(&lend, &lborder1, &tempP1);
		getCrossPoint(&lend, &lborder2, &tempP2);
		centerPEnd.x = (tempP1.x + tempP2.x) / 2;
		centerPEnd.y = (tempP1.y + tempP2.y) / 2;

		positionFlag = true;
		judgeFlag = true;

		switch (oriention) {
		case 1:
			positionFlag = centerPBegin.x < centerPEnd.x;
			break;
		case 2:
			positionFlag = centerPBegin.x > centerPEnd.x;
			break;
		case 3:
			positionFlag = centerPBegin.y > centerPEnd.y;
			break;
		case 4:
			positionFlag = centerPBegin.y < centerPEnd.y;
			break;
		default:
			return Vec4f(-1, -1, -1, -1);
		}

		judgeFlag = judgeLinePixelGrad(img, &lbegin, &lborder1, &lborder2, thres, oriention, 1, percent);

		if (!PN)  judgeFlag = !judgeFlag;
	}

	if ((positionFlag == false) && (judgeFlag == false)) {
		for (int i = 0; i < 4; ++i)
			lbegin[0] = (tempL[i] + lend[i]) / 2;
	}

	return lbegin;
}

Vec4f DetectEdges10::selectLineFromRegionStatistic(Mat img, Vec4f *l1, Vec4f *l2, Vec4f *l3, Vec4f *l4, double thres, int oriention, double percent, bool PN) {
	// oriention : 1 -> left 2 right
	//             2 -> right 2 left
	//             3 -> down 2 up
	//             4 -> up 2 down
	Point2f pbegin1, pbegin2, pend1, pend2;
	Vec4f lbegin, lend, lborder1, lborder2, tempL;
	switch (oriention) {
	case 1:
		lbegin = (*l1);
		lend = (*l3);
		lborder1 = (*l2);
		lborder2 = (*l4);
		break;
	case 2:
		lbegin = (*l3);
		lend = (*l1);
		lborder1 = (*l2);
		lborder2 = (*l4);
		break;
	case 3:
		lbegin = (*l4);
		lend = (*l2);
		lborder1 = (*l1);
		lborder2 = (*l3);
		break;
	case 4:
		lbegin = (*l2);
		lend = (*l4);
		lborder1 = (*l1);
		lborder2 = (*l3);
		break;
	default:
		return Vec4f(-1, -1, -1, -1);
	}

	tempL = lbegin;

	Point2f tempP1, tempP2;
	Point2f centerPBegin, centerPEnd;
	getCrossPoint(&lbegin, &lborder1, &tempP1);
	getCrossPoint(&lbegin, &lborder2, &tempP2);
	centerPBegin.x = (tempP1.x + tempP2.x) / 2;
	centerPBegin.y = (tempP1.y + tempP2.y) / 2;
	getCrossPoint(&lend, &lborder1, &tempP1);
	getCrossPoint(&lend, &lborder2, &tempP2);
	centerPEnd.x = (tempP1.x + tempP2.x) / 2;
	centerPEnd.y = (tempP1.y + tempP2.y) / 2;

	bool positionFlag = true;
	bool judgeFlag = true;

	switch (oriention) {
	case 1:
		positionFlag = centerPBegin.x < centerPEnd.x;
		break;
	case 2:
		positionFlag = centerPBegin.x > centerPEnd.x;
		break;
	case 3:
		positionFlag = centerPBegin.y > centerPEnd.y;
		break;
	case 4:
		positionFlag = centerPBegin.y < centerPEnd.y;
		break;
	default:
		return Vec4f(-1, -1, -1, -1);
	}

	judgeFlag = judgeLinePixelStatistic(img, &lbegin, &lborder1, &lborder2, percent, -1, 0, thres);

	if (!PN)  judgeFlag = !judgeFlag;

	while (positionFlag && judgeFlag) {
		switch (oriention) {
		case 1:
			moveLine(&lbegin, 1, 0);
			break;
		case 2:
			moveLine(&lbegin, -1, 0);
			break;
		case 3:
			moveLine(&lbegin, 0, -1);
			break;
		case 4:
			moveLine(&lbegin, 0, 1);
			break;
		default:
			return Vec4f(-1, -1, -1, -1);
		}


		getCrossPoint(&lbegin, &lborder1, &tempP1);
		getCrossPoint(&lbegin, &lborder2, &tempP2);
		centerPBegin.x = (tempP1.x + tempP2.x) / 2;
		centerPBegin.y = (tempP1.y + tempP2.y) / 2;
		getCrossPoint(&lend, &lborder1, &tempP1);
		getCrossPoint(&lend, &lborder2, &tempP2);
		centerPEnd.x = (tempP1.x + tempP2.x) / 2;
		centerPEnd.y = (tempP1.y + tempP2.y) / 2;

		positionFlag = true;
		judgeFlag = true;

		switch (oriention) {
		case 1:
			positionFlag = centerPBegin.x < centerPEnd.x;
			break;
		case 2:
			positionFlag = centerPBegin.x > centerPEnd.x;
			break;
		case 3:
			positionFlag = centerPBegin.y > centerPEnd.y;
			break;
		case 4:
			positionFlag = centerPBegin.y < centerPEnd.y;
			break;
		default:
			return Vec4f(-1, -1, -1, -1);
		}

		judgeFlag = judgeLinePixelStatistic(img, &lbegin, &lborder1, &lborder2, percent, -1, 0, thres);

		if (!PN)  judgeFlag = !judgeFlag;
	}

	if ((positionFlag == false) && (judgeFlag == false)) {
		for (int i = 0; i < 4; ++i)
			lbegin[0] = (tempL[i] + lend[i]) / 2;
	}

	return lbegin;
}

// 2018.12.08  adding
Point2f DetectEdges10::getCenterPointOfThreeLines(Vec4f *l, Vec4f *l1, Vec4f *l2) {
	Point2f p1, p2;
	getCrossPoint(l, l1, &p1);
	getCrossPoint(l, l2, &p2);

	p1.x = (p1.x + p2.x) / 2;
	p1.y = (p1.y + p2.y) / 2;

	return p1;
}

// 2018.12.11  adding
bool DetectEdges10::judgeCrossWithThreeLines(Mat img, Vec4f *l, Vec4f *l1, Vec4f *l2, uchar pixel1, uchar pixel2, int sz, double percent) {
	int halfSize = sz / 2;
	Point2f p1, p2;
	getCrossPoint(l, l1, &p1);
	getCrossPoint(l, l2, &p2);
	double percent1 = 0, percent2 = 0;
	int p1x = p1.x;
	int p1y = p1.y;
	int p2x = p2.x;
	int p2y = p2.y;

	int cnt1 = 0, cnt2 = 0;

	// pixel1 and pixel2
	for (int i = -halfSize; i <= halfSize; ++i) {
		for (int j = -halfSize; j <= halfSize; ++j) {
			if (img.at<uchar>(p1y + i, p1x + j) == pixel1) {
				cnt1++;
			}
			if (img.at<uchar>(p2y + i, p2x + j) == pixel2) {
				cnt2++;
			}
		}
	}

	percent1 = (double)cnt1 / (sz*sz);
	percent2 = (double)cnt2 / (sz*sz);

	return ((percent1 > percent) && (percent2 > percent));
}

// 2019.12.16  adding
void DetectEdges10::copyUcharArr(uchar *src, uchar *dst, int len = 256) {
	for (int i = 0;i < len;++i) {
		dst[i] = src[i];
	}
}

/*********************** utilities end ********************************/


