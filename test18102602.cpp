/* Polymorphic Operation Flow
** 1. shape *ps = new circle();
** 2. ps->rotate();
** 3. if(circle *pc = dynamic_cast<circle *>(ps)) ...
*/

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

class shape {
private:
	string name;
public:
	shape(const string &n = "") : name(n) {}
	virtual	void rotate() {
		cout << "This is shape rotate()." << endl;
	}
	virtual void print() {
		cout << "name : " << name << endl;
	}
};

class circle : public shape {
public:
	circle(const string &n = "") : shape(n) {}
	virtual void rotate() {
		cout << "This is circle rotate()." << endl;
	}
};

int main(int argc, char **argv)
{
	shape s("shape");
	circle c("circle");
	shape &ss = c;
	double *d = nullptr;
	cout << "shape > " << typeid(s).name() << endl;
	cout << "circle > " << typeid(c).name() << endl;
	cout << "shape & > " << typeid(ss).name() << endl;
	cout << "double * > " << typeid(d).name() << endl;
	cout << "> " << typeid(typeid(s).name()).name() << endl;
	string tempName = typeid(s).name();
	cout << "> " << typeid(tempName).name() << endl;
	s.print();
	c.print();
	ss.print();
	//
	char cc;
	cin >> cc;
	return 0;
}
