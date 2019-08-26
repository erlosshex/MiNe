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
	void getCrossPoint(const cv::Vec4f &l1, const cv::Vec4f &l2, cv::Point2f &p);
	void getCrossPoint(const cv::Vec4f &l1, const cv::Vec4f &l2, cv::Point2d &p);
	// move a line, that is represented by vector model
	void moveLine(cv::Vec4f *l, float distX, float distY);
	// 2019.08.23 over
		// get the cross points of a line and a rectangle
	std::vector<cv::Point2d> getCrossPointsOfLineRect(const cv::Vec4f &l, const cv::Rect &rect);
	// convert two line segment points into a line with vector
	void convertLinePoint2Vec(const cv::Point *p1, const cv::Point *p2, cv::Vec4f *l);
	void convertLinePoint2Vec(const cv::Point2f *p1, const cv::Point2f *p2, cv::Vec4f *l);
	void convertLinePoint2Vec(const cv::Point2d *p1, const cv::Point2d *p2, cv::Vec4f *l);
	// get the four points of a rectangle roi
	void getVerticesOfRect(const cv::Rect &rect, std::vector<cv::Point> &vertices);
	void getVerticesOfRect(const cv::Rect &rect, std::vector<cv::Point2f> &vertices);
	void getVerticesOfRect(const cv::Rect &rect, std::vector<cv::Point2d> &vertices);
	// get the lines of a rectangle roi
	void getLinesOfRectVertices(const std::vector<cv::Point> &vertices, std::vector<cv::Vec4f> &lines);
	void getLinesOfRectVertices(const std::vector<cv::Point2f> &vertices, std::vector<cv::Vec4f> &lines);
	void getLinesOfRectVertices(const std::vector<cv::Point2d> &vertices, std::vector<cv::Vec4f> &lines);
	void getLinesOfRect(const cv::Rect &rect, std::vector<cv::Vec4f> &lines);
	// check point of rect roi
	int checkPointInRoiOfRect(const cv::Point &p, const cv::Rect &rect, double distThres = 0.0);
	int checkPointInRoiOfRect(const cv::Point2f &p, const cv::Rect &rect, double distThres = 0.0);
	int checkPointInRoiOfRect(const cv::Point2d &p, const cv::Rect &rect, double distThres = 0.0);
	// template match
	// match the pixels of a region
	cv::Mat TemplateMatch(cv::Mat &img, cv::Mat &templ, int match_method, cv::Point &matchLoc, double &matchValue);
	// match the shape of  a region
	std::vector<cv::Point> ShapeMatch(cv::Mat &img, cv::Mat &templ, int match_method, int thres, cv::Point &matchLoc, double &matchValue);
	int ShapeMatch(std::vector<cv::Point> &con, std::vector<cv::Point> &templ_con, int match_method);
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

void ImageUtility::getCrossPoint(const cv::Vec4f & l1, const cv::Vec4f & l2, cv::Point2f & p)
{
	double vx1 = l1[0];
	double vy1 = l1[1];
	double x1 = l1[2];
	double y1 = l1[3];

	double vx2 = l2[0];
	double vy2 = l2[1];
	double x2 = l2[2];
	double y2 = l2[3];

	double operation = vx1 * vy2 - vx2 * vy1;

	if (operation != 0.0) {
		double x = (x2*vx1*vy2 - y2 * vx1*vx2 - x1 * vx2*vy1 + y1 * vx1*vx2) / operation;
		double y = (x2*vy1*vy2 - y2 * vx2*vy1 - x1 * vy1*vy2 + y1 * vx1*vy2) / operation;

		p.x = x;
		p.y = y;
	}
	else {
		p.x = -1;
		p.y = -1;
	}
}

void ImageUtility::getCrossPoint(const cv::Vec4f & l1, const cv::Vec4f & l2, cv::Point2d & p)
{
	double vx1 = l1[0];
	double vy1 = l1[1];
	double x1 = l1[2];
	double y1 = l1[3];

	double vx2 = l2[0];
	double vy2 = l2[1];
	double x2 = l2[2];
	double y2 = l2[3];

	double x = (x2*vx1*vy2 - y2 * vx1*vx2 - x1 * vx2*vy1 + y1 * vx1*vx2) / (vx1*vy2 - vx2 * vy1);
	double y = (x2*vy1*vy2 - y2 * vx2*vy1 - x1 * vy1*vy2 + y1 * vx1*vy2) / (vx1*vy2 - vx2 * vy1);

	p.x = x;
	p.y = y;
}

void ImageUtility::moveLine(cv::Vec4f *l, float distX, float distY)
{
	if (abs(distX) < 0.01 && abs(distY) < 0.01)
		return;

	(*l)[2] = (*l)[2] + distX;
	(*l)[3] = (*l)[3] + distY;
}

std::vector<cv::Point2d> ImageUtility::getCrossPointsOfLineRect(const cv::Vec4f & l, const cv::Rect & rect)
{
	std::vector<cv::Point2d> res;

	try {
		std::vector<cv::Point2d> vertices;
		getVerticesOfRect(rect, vertices);
		std::vector<cv::Vec4f> lines;
		getLinesOfRectVertices(vertices, lines);

		cv::Vec4f l1 = lines[0];
		cv::Vec4f l2 = lines[1];
		cv::Vec4f l3 = lines[2];
		cv::Vec4f l4 = lines[3];
		cv::Point2d p1, p2, p3, p4;

		getCrossPoint(l1, l, p1);
		getCrossPoint(l2, l, p2);
		getCrossPoint(l3, l, p3);
		getCrossPoint(l4, l, p4);

		if (0 == checkPointInRoiOfRect(p1, rect, 0.1)) {
			res.push_back(p1);
		}
		if (0 == checkPointInRoiOfRect(p2, rect, 0.1)) {
			res.push_back(p2);
		}
		if (0 == checkPointInRoiOfRect(p3, rect, 0.1)) {
			res.push_back(p3);
		}
		if (0 == checkPointInRoiOfRect(p4, rect, 0.1)) {
			res.push_back(p4);
		}
	}
	catch (std::exception &e) {
		res.clear();
	}

	return res;
}

void ImageUtility::convertLinePoint2Vec(const cv::Point * p1, const cv::Point * p2, cv::Vec4f * l)
{
	double p1x = p1->x;
	double p1y = p1->y;
	double p2x = p2->x;
	double p2y = p2->y;

	double vx = p2x - p1x;
	double vy = p2y - p1y;

	(*l)[2] = p1x;
	(*l)[3] = p1y;

	(*l)[0] = vx / sqrt(vx*vx + vy * vy);
	(*l)[1] = vy / sqrt(vx*vx + vy * vy);
}

void ImageUtility::convertLinePoint2Vec(const cv::Point2f *p1, const cv::Point2f *p2, cv::Vec4f *l)
{
	double p1x = p1->x;
	double p1y = p1->y;
	double p2x = p2->x;
	double p2y = p2->y;

	double vx = p2x - p1x;
	double vy = p2y - p1y;

	(*l)[2] = p1x;
	(*l)[3] = p1y;

	(*l)[0] = vx / sqrt(vx*vx + vy * vy);
	(*l)[1] = vy / sqrt(vx*vx + vy * vy);
}

void ImageUtility::convertLinePoint2Vec(const cv::Point2d *p1, const cv::Point2d *p2, cv::Vec4f *l)
{
	double p1x = p1->x;
	double p1y = p1->y;
	double p2x = p2->x;
	double p2y = p2->y;

	double vx = p2x - p1x;
	double vy = p2y - p1y;

	(*l)[2] = p1x;
	(*l)[3] = p1y;

	(*l)[0] = vx / sqrt(vx*vx + vy * vy);
	(*l)[1] = vy / sqrt(vx*vx + vy * vy);
}

void ImageUtility::getVerticesOfRect(const cv::Rect & rect, std::vector<cv::Point>& vertices)
{
	int r_x = rect.x;
	int r_y = rect.y;
	int r_w = rect.width;
	int r_h = rect.height;

	vertices.clear();

	vertices.push_back(cv::Point(r_x, r_y));
	vertices.push_back(cv::Point(r_x + r_w - 1, r_y));
	vertices.push_back(cv::Point(r_x + r_w - 1, r_y + r_h - 1));
	vertices.push_back(cv::Point(r_x, r_y + r_h - 1));
}

void ImageUtility::getVerticesOfRect(const cv::Rect & rect, std::vector<cv::Point2f>& vertices)
{
	int r_x = rect.x;
	int r_y = rect.y;
	int r_w = rect.width;
	int r_h = rect.height;

	vertices.clear();

	vertices.push_back(cv::Point2f(r_x, r_y));
	vertices.push_back(cv::Point2f(r_x + r_w - 1, r_y));
	vertices.push_back(cv::Point2f(r_x + r_w - 1, r_y + r_h - 1));
	vertices.push_back(cv::Point2f(r_x, r_y + r_h - 1));
}

void ImageUtility::getVerticesOfRect(const cv::Rect & rect, std::vector<cv::Point2d>& vertices)
{
	int r_x = rect.x;
	int r_y = rect.y;
	int r_w = rect.width;
	int r_h = rect.height;

	vertices.clear();

	vertices.push_back(cv::Point2d(r_x, r_y));
	vertices.push_back(cv::Point2d(r_x + r_w - 1, r_y));
	vertices.push_back(cv::Point2d(r_x + r_w - 1, r_y + r_h - 1));
	vertices.push_back(cv::Point2d(r_x, r_y + r_h - 1));
}

void ImageUtility::getLinesOfRectVertices(const std::vector<cv::Point>& vertices, std::vector<cv::Vec4f>& lines)
{
	lines.clear();

	cv::Vec4f l1, l2, l3, l4;

	convertLinePoint2Vec(&vertices[3], &vertices[0], &l1);
	convertLinePoint2Vec(&vertices[0], &vertices[1], &l2);
	convertLinePoint2Vec(&vertices[1], &vertices[2], &l3);
	convertLinePoint2Vec(&vertices[2], &vertices[3], &l4);

	lines.push_back(l1);
	lines.push_back(l2);
	lines.push_back(l3);
	lines.push_back(l4);
}

void ImageUtility::getLinesOfRectVertices(const std::vector<cv::Point2f>& vertices, std::vector<cv::Vec4f>& lines)
{
	lines.clear();

	cv::Vec4f l1, l2, l3, l4;

	convertLinePoint2Vec(&vertices[3], &vertices[0], &l1);
	convertLinePoint2Vec(&vertices[0], &vertices[1], &l2);
	convertLinePoint2Vec(&vertices[1], &vertices[2], &l3);
	convertLinePoint2Vec(&vertices[2], &vertices[3], &l4);

	lines.push_back(l1);
	lines.push_back(l2);
	lines.push_back(l3);
	lines.push_back(l4);
}

void ImageUtility::getLinesOfRectVertices(const std::vector<cv::Point2d>& vertices, std::vector<cv::Vec4f>& lines)
{
	lines.clear();

	cv::Vec4f l1, l2, l3, l4;

	convertLinePoint2Vec(&vertices[3], &vertices[0], &l1);
	convertLinePoint2Vec(&vertices[0], &vertices[1], &l2);
	convertLinePoint2Vec(&vertices[1], &vertices[2], &l3);
	convertLinePoint2Vec(&vertices[2], &vertices[3], &l4);

	lines.push_back(l1);
	lines.push_back(l2);
	lines.push_back(l3);
	lines.push_back(l4);
}

void ImageUtility::getLinesOfRect(const cv::Rect & rect, std::vector<cv::Vec4f>& lines)
{
	lines.clear();
	std::vector<cv::Point2d> vertices;
	getVerticesOfRect(rect, vertices);
	getLinesOfRectVertices(vertices, lines);
}

int ImageUtility::checkPointInRoiOfRect(const cv::Point & p, const cv::Rect & rect, double distThres)
{
	int res = -1;

	int range_x_0 = rect.x;
	int range_x_1 = rect.x + rect.width - 1;
	int range_y_0 = rect.y;
	int range_y_1 = rect.y + rect.height - 1;

	if (abs(p.x - range_x_0) <= distThres ||
		abs(p.x - range_x_1) <= distThres ||
		abs(p.y - range_y_0) <= distThres ||
		abs(p.y - range_y_1) <= distThres) {
		res = 0;
	}
	else {
		if (p.x > range_x_0 && p.x < range_x_1 &&
			p.y > range_y_0 && p.y < range_y_1) {
			res = 1;
		}
		else {
			res = -1;
		}
	}

	return res;
}

int ImageUtility::checkPointInRoiOfRect(const cv::Point2f & p, const cv::Rect & rect, double distThres)
{
	int res = -1;

	int range_x_0 = rect.x;
	int range_x_1 = rect.x + rect.width - 1;
	int range_y_0 = rect.y;
	int range_y_1 = rect.y + rect.height - 1;

	if (abs(p.x - range_x_0) <= distThres ||
		abs(p.x - range_x_1) <= distThres ||
		abs(p.y - range_y_0) <= distThres ||
		abs(p.y - range_y_1) <= distThres) {
		res = 0;
	}
	else {
		if (p.x > range_x_0 && p.x < range_x_1 &&
			p.y > range_y_0 && p.y < range_y_1) {
			res = 1;
		}
		else {
			res = -1;
		}
	}

	return res;
}

int ImageUtility::checkPointInRoiOfRect(const cv::Point2d & p, const cv::Rect & rect, double distThres)
{
	int res = -1;

	int range_x_0 = rect.x;
	int range_x_1 = rect.x + rect.width - 1;
	int range_y_0 = rect.y;
	int range_y_1 = rect.y + rect.height - 1;

	if (abs(p.x - range_x_0) <= distThres ||
		abs(p.x - range_x_1) <= distThres ||
		abs(p.y - range_y_0) <= distThres ||
		abs(p.y - range_y_1) <= distThres) {
		res = 0;
	}
	else {
		if (p.x > range_x_0 && p.x < range_x_1 &&
			p.y > range_y_0 && p.y < range_y_1) {
			res = 1;
		}
		else {
			res = -1;
		}
	}

	return res;
}

cv::Mat ImageUtility::TemplateMatch(cv::Mat & img, cv::Mat & templ, int match_method, cv::Point & matchLoc, double & matchValue)
{
	bool use_mask = false;

	cv::Mat result, mask;

	bool method_accepts_mask = (cv::TemplateMatchModes::TM_SQDIFF == match_method || match_method == cv::TemplateMatchModes::TM_CCORR_NORMED);

	if (use_mask && method_accepts_mask){
		matchTemplate(img, templ, result, match_method, mask);
	}
	else{
		matchTemplate(img, templ, result, match_method);
	}
	/// Localizing the best match with minMaxLoc
	double minVal;
	double maxVal;
	cv::Point minLoc;
	cv::Point maxLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == cv::TemplateMatchModes::TM_SQDIFF || match_method == cv::TemplateMatchModes::TM_SQDIFF_NORMED){
		matchLoc = minLoc;
		matchValue = minVal;
	}
	else{
		matchLoc = maxLoc;
		matchValue = maxVal;
	}

	return result;
}

std::vector<cv::Point> ImageUtility::ShapeMatch(cv::Mat & img, cv::Mat & templ, int match_method, int thres, cv::Point & matchLoc, double & matchValue)
{
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarcy;

	cv::findContours(templ, contours, hierarcy, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_NONE);

	cv::Mat binary_image;
	cv::threshold(img, binary_image, thres, 255, cv::ThresholdTypes::THRESH_BINARY);

	std::vector<std::vector<cv::Point> > contours2;
	std::vector<cv::Vec4i> hierarcy2;

	cv::findContours(binary_image, contours2, hierarcy2, cv::RetrievalModes::RETR_TREE, cv::ContourApproximationModes::CHAIN_APPROX_NONE);

	double bestMatch = std::numeric_limits<double>::max();
	double tempMatch = 0;

	int bestIndex = -1;
	int tempIndex = -1;

	for (auto &con : contours2) {
		tempIndex++;
		tempMatch = cv::matchShapes(contours[0], con, match_method, 0.0);
		if (tempMatch < bestMatch) {
			bestMatch = tempMatch;
			bestIndex = tempIndex;
		}
	}

	std::vector<cv::Point> res = contours2[bestIndex];

	cv::Moments m = cv::moments(res);
	matchLoc.x = m.m10 / m.m00;
	matchLoc.y = m.m01 / m.m00;

	matchValue = bestMatch;
	
	return res;
}

int ImageUtility::ShapeMatch(std::vector<cv::Point>& con, std::vector<cv::Point>& templ_con, int match_method)
{
	double matchScore = cv::matchShapes(templ_con, con, match_method, 0.0);
	return matchScore;
}

int main(int argc, char **argv)
{
	std::cout << "hello, world!" << std::endl;
	return 0;
}
