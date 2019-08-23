#include <iostream>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

namespace ImageUtility {
	// rotate mat --- clockwise 90
	cv::Mat matRotateClockWise90(cv::Mat _src);
	// rotate mat --- clockwise 180
	cv::Mat matRotateClockWise180(cv::Mat _src);
	// rotate mat --- clockwise 270
	cv::Mat matRotateClockWise270(cv::Mat _src);
	// move rectangle
	cv::Rect moveRect(const cv::Rect &rect, int x, int y);
	cv::Rect moveRect(const cv::Rect &rect, const cv::Point &offset);
	cv::Rect moveRect(cv::Rect *rect_p, int x, int y);
	cv::Rect moveRect(cv::Rect *rect_p, const cv::Point &offset);
	bool moveRectATO(cv::Rect &rect, int x, int y);
	bool moveRectATO(cv::Rect &rect, const cv::Point &offset);
	bool moveRectATO(cv::Rect *rect_p, int x, int y);
	bool moveRectATO(cv::Rect *rect_p, const cv::Point &offset);
	// get the cross point of two lines
	void getCrossPoint(cv::Vec4f *l1, cv::Vec4f *l2, cv::Point2f *p);
	void getCrossPoint(cv::Vec4f *l1, cv::Vec4f *l2, cv::Point2d *p);
	// move a line, that is represented by vector model
	void moveLine(cv::Vec4f *l, float distX, float distY);
	// 2019.08.23 over

}

//**************************************************************//

cv::Mat ImageUtility::matRotateClockWise90(cv::Mat _src) {
	cv::Mat src_copy = _src.clone();
	if (src_copy.empty()) {
		std::cout << "RotateMat src is empty!" << std::endl;
	}
	transpose(src_copy, src_copy);
	flip(src_copy, src_copy, 1);
	return src_copy;
}

cv::Mat ImageUtility::matRotateClockWise180(cv::Mat _src) {
	cv::Mat src_copy = _src.clone();
	if (src_copy.empty()) {
		std::cout << "RotateMat src is empty!" << std::endl;
	}
	flip(src_copy, src_copy, 0);
	flip(src_copy, src_copy, 0);
	return src_copy;
}

cv::Mat ImageUtility::matRotateClockWise270(cv::Mat _src) {
	cv::Mat src_copy = _src.clone();
	if (src_copy.empty()) {
		std::cout << "RotateMat src is empty!" << std::endl;
	}
	transpose(src_copy, src_copy);
	flip(src_copy, src_copy, 0);
	return src_copy;
}

cv::Rect ImageUtility::moveRect(const cv::Rect &rect, int x, int y) {
	cv::Rect res = rect;

	try {
		res.x += x;
		res.y += y;
	}
	catch (std::exception &e) {
		std::cout << "Exception : " << e.what() << std::endl;
		res = rect;
	}

	return res;
}

cv::Rect ImageUtility::moveRect(const cv::Rect &rect, const cv::Point &offset) {
	cv::Rect res = rect;

	try {
		res.x += offset.x;
		res.y += offset.y;
	}
	catch (std::exception &e) {
		std::cout << "Exception : " << e.what() << std::endl;
		res = rect;
	}

	return res;
}

cv::Rect ImageUtility::moveRect(cv::Rect *rect_p, int x, int y) {
	cv::Rect res = (*rect_p);

	try {
		res.x += x;
		res.y += y;
	}
	catch (std::exception &e) {
		std::cout << "Exception : " << e.what() << std::endl;
		res = (*rect_p);
	}

	return res;
}

cv::Rect ImageUtility::moveRect(cv::Rect *rect_p, const cv::Point &offset) {
	cv::Rect res = (*rect_p);

	try {
		res.x += offset.x;
		res.y += offset.y;
	}
	catch (std::exception &e) {
		std::cout << "Exception : " << e.what() << std::endl;
		res = (*rect_p);
	}

	return res;
}

bool ImageUtility::moveRectATO(cv::Rect &rect, int x, int y) {
	bool res = false;

	try {
		rect.x += x;
		rect.y += y;
		res = true;
	}
	catch (std::exception &e) {
		std::cout << "Exception : " << e.what() << std::endl;
		res = false;
	}

	return res;
}

bool ImageUtility::moveRectATO(cv::Rect &rect, const cv::Point &offset) {
	bool res = false;

	try {
		rect.x += offset.x;
		rect.y += offset.y;
		res = true;
	}
	catch (std::exception &e) {
		std::cout << "Exception : " << e.what() << std::endl;
		res = false;
	}

	return res;
}

bool ImageUtility::moveRectATO(cv::Rect *rect_p, int x, int y) {
	bool res = false;

	try {
		rect_p->x += x;
		rect_p->y += y;
		res = true;
	}
	catch (std::exception &e) {
		std::cout << "Exception : " << e.what() << std::endl;
		res = false;
	}

	return res;
}

bool ImageUtility::moveRectATO(cv::Rect *rect_p, const cv::Point &offset) {
	bool res = false;

	try {
		rect_p->x += offset.x;
		rect_p->y += offset.y;
		res = true;
	}
	catch (std::exception &e) {
		std::cout << "Exception : " << e.what() << std::endl;
		res = false;
	}

	return res;
}

void ImageUtility::getCrossPoint(cv::Vec4f *l1, cv::Vec4f *l2, cv::Point2f *p)
{
	double vx1 = (*l1)[0];
	double vy1 = (*l1)[1];
	double x1 = (*l1)[2];
	double y1 = (*l1)[3];

	double vx2 = (*l2)[0];
	double vy2 = (*l2)[1];
	double x2 = (*l2)[2];
	double y2 = (*l2)[3];

	double operation = vx1 * vy2 - vx2 * vy1;

	if (operation != 0.0) {
		double x = (x2*vx1*vy2 - y2 * vx1*vx2 - x1 * vx2*vy1 + y1 * vx1*vx2) / operation;
		double y = (x2*vy1*vy2 - y2 * vx2*vy1 - x1 * vy1*vy2 + y1 * vx1*vy2) / operation;

		p->x = x;
		p->y = y;
	}
	else {
		p->x = -1;
		p->y = -1;
	}
}

void ImageUtility::getCrossPoint(cv::Vec4f *l1, cv::Vec4f *l2, cv::Point2d *p)
{
	double vx1 = (*l1)[0];
	double vy1 = (*l1)[1];
	double x1 = (*l1)[2];
	double y1 = (*l1)[3];

	double vx2 = (*l2)[0];
	double vy2 = (*l2)[1];
	double x2 = (*l2)[2];
	double y2 = (*l2)[3];

	double x = (x2*vx1*vy2 - y2 * vx1*vx2 - x1 * vx2*vy1 + y1 * vx1*vx2) / (vx1*vy2 - vx2 * vy1);
	double y = (x2*vy1*vy2 - y2 * vx2*vy1 - x1 * vy1*vy2 + y1 * vx1*vy2) / (vx1*vy2 - vx2 * vy1);

	p->x = x;
	p->y = y;
}

void ImageUtility::moveLine(cv::Vec4f *l, float distX, float distY)
{
	if (abs(distX) < 0.01 && abs(distY) < 0.01)
		return;

	(*l)[2] = (*l)[2] + distX;
	(*l)[3] = (*l)[3] + distY;
}

int main(int argc, char **argv)
{
	std::cout << "hello, world!" << std::endl;
	return 0;
}
