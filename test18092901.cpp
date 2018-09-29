//此程序是进行坐标系的转换
//步骤1：根据三个圆心和法兰中心得到CCD与机器人的坐标转换
//步骤2：在做模板标定的时候，记录下特征点的位置和法兰中心的位置，构建成移动向量，并记录下各自的角度
//步骤3: 根据做好的模板标定，在检测时注意检测到的特征与模板特征之间的旋转和平移关系，以此来更新移动向量，得到新的法兰中心的CCD坐标
//       再逆转换成世界坐标（机器人坐标），并加上角度
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include <iterator>

using namespace std;

const double pi = acos(-1);

class Pos {
public:
	Pos(double xx = 0, double yy = 0, double ang = 0) : x(xx), y(yy), angle(ang) {}
	Pos(const Pos &p) : x(p.x), y(p.y), angle(p.angle) {}
	~Pos() {}
	double x;
	double y;
	double angle;
	void setX(double xx) { x = xx; }
	void setY(double yy) { y = yy; }
	void setAngle(double ang) { angle = ang; }
	double getX() const { return x; }
	double getX() { return x; }
	double getY() const { return y; }
	double getY() { return y; }
	double getAngle() const { return angle; }
	double getAngle() { return angle; }
};

//CCD检测的像素坐标和角度
typedef Pos PixelPos;
//Robot移动的世界坐标和角度
typedef Pos RobotPos;

//Pos转vector
void pos2Vector(const Pos &p, vector<double> &v) {
	if (!v.empty())  v.clear();
	v.push_back(p.x);
	v.push_back(p.y);
	v.push_back(1);
}

//vector转Pos
void vector2Pos(const vector<double> &v, Pos &p) {
	p.x = v[0];
	p.y = v[1];
}

//array转vector
void array2Mat(double **arr, vector<vector<double>> &v, int row, int col) {
	if (!v.empty())  v.clear();
	for (int r = 0; r < row; ++r) {
		vector<double> temp;
		for (int c = 0; c < col; ++c) {
			temp.push_back(*((double *)arr+r*col+c));
		}
		v.push_back(temp);
	}
}

//vector复制到vector
void vec2Vec(vector<double> &v1, vector<double> &v2) {
	if (&v1 == &v2) {
		vector<double> v;
		vector<double>::iterator b = v1.begin();
		vector<double>::iterator e = v1.end();
		for (; b != e; ++b) {
			v.push_back(*b);
		}
		if (!v2.empty())  v2.clear();
		b = v.begin();
		e = v.end();
		for (; b != e; ++b) {
			v2.push_back(*b);
		}
	}
	else {
		if (!v2.empty())  v2.clear();
		vector<double>::iterator b = v1.begin();
		vector<double>::iterator e = v1.end();
		for (; b != e; ++b) {
			v2.push_back(*b);
		}
	}
	
}

//根据向量移动点
Pos movePosWithVec(Pos &p, Pos &v) {
	Pos newP;
	newP.x = p.x + v.x;
	newP.y = p.y + v.y;
	return newP;
}

//旋转向量
Pos rotateVec(Pos &p, double ang, bool flag) {
//flag表示是否使用的是弧度值
	if (flag == false) ang = ang / 180 * pi;
	double polarL = sqrt(pow(p.x, 2) + pow(p.y, 2));
	double polarAngle = (fabs(p.x) < 1.0E-8) ? ((p.y>0) ? (pi/2) : (-pi/2)):(atan(p.y / p.x));
	polarAngle += ang;
	Pos newP;
	newP.x = polarL*cos(polarAngle);
	newP.y = polarL*sin(polarAngle);
	return newP;
}

//Mat复制到Mat
void mat2Mat(vector<vector<double>> &m1, vector<vector<double>> &m2) {
	if (&m1 == &m2) {
		vector<vector<double>> m;
		vector<vector<double>>::iterator b = m1.begin();
		vector<vector<double>>::iterator e = m1.end();
		for (; b != e; ++b) {
			m.push_back(*b);
		}
		if (!m2.empty())  m2.clear();
		b = m.begin();
		e = m.end();
		for (; b != e; ++b) {
			m2.push_back(*b);
		}
	}
	else {
		if (!m2.empty())  m2.clear();
		vector<vector<double>>::iterator b = m1.begin();
		vector<vector<double>>::iterator e = m1.end();
		for (; b != e; ++b) {
			m2.push_back(*b);
		}
	}
}

//矩阵转置
void tranMat(vector<vector<double>> &m, vector<vector<double>> &r) {
	if (&m == &r) {
		vector<vector<double>> tempM;
		mat2Mat(m, tempM);
		r.clear();
		int row = tempM.size();
		int col = tempM[0].size();
		for (int c = 0; c < col; ++c) {
			vector<double> temp;
			for (int r = 0; r < row; ++r) {
				temp.push_back(tempM[r][c]);
			}
			r.push_back(temp);
		}
	}
	else {
		int row = m.size();
		int col = m[0].size();
		for (int c = 0; c < col; ++c) {
			vector<double> temp;
			for (int r = 0; r < row; ++r) {
				temp.push_back(m[r][c]);
			}
			r.push_back(temp);
		}
	}

}

//3x3矩阵的逆矩阵
void invMat3x3(vector<vector<double>> &mm, vector<vector<double>> &r) {
	vector<vector<double>> m;
	mat2Mat(mm, m);
	if (!r.empty())  r.clear();
	double n1 = m[0][2] * m[1][1] * m[2][0];
	double n2 = m[0][1] * m[1][0] * m[2][2];
	double n3 = m[0][0] * m[1][2] * m[2][1];
	double p1 = m[0][0] * m[1][1] * m[2][2];
	double p2 = m[0][1] * m[1][2] * m[2][0];
	double p3 = m[0][2] * m[1][0] * m[2][1];
	double det = p1 + p2 + p3 - n1 - n2 - n3;
	double d00 = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / det;
	double d01 = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) / det;
	double d02 = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / det;
	double d10 = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) / det;
	double d11 = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / det;
	double d12 = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) / det;
	double d20 = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / det;
	double d21 = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) / det;
	double d22 = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / det;
	r.push_back(vector<double>{d00, d01, d02});
	r.push_back(vector<double>{d10, d11, d12});
	r.push_back(vector<double>{d20, d21, d22});
	tranMat(r, r);
}

//3*n与n*3矩阵相乘
void mulMat(vector<vector<double>> &mm1, vector<vector<double>> &mm2, vector<vector<double>> &res) {
	vector<vector<double>> m1;
	vector<vector<double>> m2;
	mat2Mat(mm1, m1);
	mat2Mat(mm2, m2);
	if (!res.empty())  res.clear();
	int row1 = m1.size();
	int col1 = m1[0].size();
	int row2 = m2.size();
	int col2 = m2[0].size();
	if (col1 != row2)  return;
	for (int r = 0; r < row1; ++r) {
		vector<double> temp;
		for (int c = 0; c < col2; ++c) {
			double s = 0;
			for (int k = 0; k < col1; ++k) {
				s += m1[r][k] * m2[k][c];
			}
			temp.push_back(s);
		}
		res.push_back(temp);
	}
}

//打印矩阵
void printMat(vector<vector<double>> &m) {
	int row = m.size();
	int col = m[0].size();
	for (int r = 0; r < row; ++r) {
		for (int c = 0; c < col; ++c) {
			cout << m[r][c] << " ";
		}
		cout << "\n";
	}
	cout << endl;
}

//打印坐标点
void printPos(Pos &p) {
	cout << "Pos:" << endl;
	cout << "    x : " << p.x << endl;
	cout << "    y : " << p.y << endl;
	cout << "angle : " << p.angle << endl;
}

//计算两个点之间的距离
double calc2PointDist(const Pos *p1, const Pos *p2) {
	return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}

double calc2PointDist(const Pos &p1, const Pos &p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//计算像素和世界坐标的比例, Pixel/Robot
double scalePixelWithRobot(const vector<Pos> &pv, const vector<Pos> &rv, const vector<pair<int, int>> &match) {
	double pdist = calc2PointDist(&pv[match[0].first],&pv[match[1].first]);
	double rdist = calc2PointDist(&rv[match[0].second], &rv[match[1].second]);
	if (rdist < 0.00001)  return -1;
	return (pdist/rdist);
}

void calib(const vector<Pos> &pv, const vector<Pos> &rv, vector<vector<double>> &tran) {
	//创建像素坐标矩阵
	vector<vector<double>> pm;
	vector<vector<double>>::iterator b = pv.begin();
	vector<vector<double>>::iterator e = pv.end();
	for (; b != e; ++b) {
		vector<double> temp ={(*b).x,(*b).y,1};
		pm.push_back(temp);
	}
	//创建世界坐标矩阵
	vector<vector<double>> rm;
	b = rv.begin();
	e = rv.end();
	for (; b != e; ++b) {
		vector<double> temp = { (*b).x,(*b).y,1 };
		rm.push_back(temp);
	}
	//建立坐标系联系
	vector<vector<double>> tempM;
	tranMat(pm,tempM);
	mulMat(tempM,pm,tempM);
	invMat3x3(tempM,tempM);
	mulMat(tempM, pm, tempM);
	mulMat(tempM, rm, tran);
}

Pos calcCirclePos(Pos &p1, Pos &p2, Pos &p3) {
	double x1 = p1.x;
	double y1 = p1.y;
	double x2 = p2.x;
	double y2 = p2.y;
	double x3 = p3.x;
	double y3 = p3.y;
	double x4 = (x1 + x2) / 2;
	double y4 = (y1 + y2) / 2;
	double x5 = (x2 + x3) / 2;
	double y5 = (y2 + y3) / 2;
	double a = x2 - x1;
	double b = y2 - y1;
	double c = x3 - x2;
	double d = y3 - y2;
	double e = a*x4 + b*y4;
	double f = c*x5 + d*y5;
	Pos cir;
	cir.x = (d*e - b*f) / (a*d - b*c);
	cir.y = (c*e - a*f) / (b*c - a*d);
	return cir;
}

int main(int argc, char **argv)
{
	//codes for testing
	pair<int, int> temp = make_pair(1, 2);
	cout << "pair fist : " << temp.first << endl;
	cout << "pair second : " << temp.second << endl;
	//
	cout << string(50, '*') << endl;
	//
	Pos* p1 = new PixelPos(1, 2, 3);
	Pos* p2 = new PixelPos(2, 3, 4);
	Pos* p3 = new PixelPos(3, 4, 5);
	Pos* p4 = new PixelPos(4, 5, 6);
	Pos* p5 = new PixelPos(5, 6, 7);
	Pos* p6 = new PixelPos(6, 7, 8);
	Pos* p7 = new PixelPos(7, 8, 9);
	Pos* p8 = new PixelPos(8, 9, 0);
	vector<Pos> pv;
	pv.push_back(*p1);
	pv.push_back(*p2);
	pv.push_back(*p3);
	pv.push_back(*p4);
	vector<Pos> rv;
	rv.push_back(*p5);
	rv.push_back(*p6);
	rv.push_back(*p7);
	rv.push_back(*p8);
	vector<pair<int, int> > match{ {1,1},{2,3} };
	cout << "scale : " << scalePixelWithRobot(pv, rv, match) << endl;
	printMat(vector<vector<double>>{ {1, 2, 3}, { 2,3,4 }});
	//
	cout << string(50, '*') << endl;
	//
	double arr[][3] = { {1,2,3},{4,5,6},{7,8,9} };
	double arr1[][2] = { {1,2},{2,3},{3,4} };
	vector<vector<double>> m;
	vector<vector<double>> m1;
	array2Mat((double **)arr, m, 3, 3);
	array2Mat((double **)arr1, m1, 3, 2);
	vector<vector<double>> m2;
	printMat(m1);
	mulMat(m, m1, m1);
	printMat(m);
	printMat(m1);
	Pos circleR = calcCirclePos(Pos(1, 2), Pos(2, 3), Pos(4, 2));
	printPos(circleR);
	cout << "R : " << calc2PointDist(Pos(1, 2), circleR) << endl;
	//
	double arr3[][3] = { { 1,2,5 },{ 4,3,6 },{ 7,8,9 } };
	vector<vector<double>> m3;
	vector<vector<double>> m4;
	vector<vector<double>> m5;
	array2Mat((double **)arr3, m3, 3, 3);
	printMat(m3);
	invMat3x3(m3, m4);
	printMat(m4);
	mulMat(m3, m4, m5);
	printMat(m5);
	//
	cout << string(50, '-') << endl;
	printPos(rotateVec(Pos(1, 3), 30, 0));
	cout << sqrt(pow(rotateVec(Pos(1, 3), 30, 0).x, 2)+pow(rotateVec(Pos(1, 3), 30, 0).y, 2)) << endl;
	cout << sqrt(10) << endl;
	//
	char c;
	cin >> c;
	return 0;
}
