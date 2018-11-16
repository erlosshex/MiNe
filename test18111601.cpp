#include "highgui/highgui.hpp"    
#include "imgproc/imgproc.hpp"    
#include "iostream"  

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	Mat imageSource = imread("1.jpg", 0);
	imshow("Source Image", imageSource);
	vector<Point> contour;
	Point p1(800,400), p2(1100,605), p3(902,970), p4(802,780);//随意给了4个点
	contour.push_back(p1);
	contour.push_back(p2);
	contour.push_back(p3);
	contour.push_back(p4);
	RotatedRect rect = minAreaRect(contour);//外接矩形
	Point2f vertices[4];
	rect.points(vertices);//外接矩形的4个顶点
	for (int i = 0; i < 4; i++)//画矩形
		line(imageSource, vertices[i], vertices[(i + 1) % 4], Scalar(255, 0, 0));

	/*Rect brect = rect.boundingRect();
	rectangle(imageSource, brect, Scalar(255, 0, 0));*/
	imshow("Source Image1", imageSource);
	Point2f center = rect.center;//外接矩形中心点坐标
	Mat rot_mat = getRotationMatrix2D(center, rect.angle, 1.0);//求旋转矩阵
	Mat rot_image;
	Size dst_sz(imageSource.size());
	warpAffine(imageSource, rot_image, rot_mat, dst_sz);//原图像旋转
	imshow("rot_image", rot_image);
	Mat result1 = rot_image(Rect(center.x - (rect.size.width / 2), center.y - (rect.size.height/2), rect.size.width, rect.size.height));//提取ROI
	imshow("result", result1);

	waitKey(0);
	return 0;
}
