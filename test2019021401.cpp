#include <iostream>

using namespace std;

class A {
public:
	A() {
		cout << "gou zao A" << endl;
	}
	virtual ~A() {
		cout << "xi gou A" << endl; // 能正常调用
	}
	/*~A() {
		cout << "xi gou A" << endl; // 不能正常调用
	}*/
	virtual void print() {
		cout << "class A" << endl;
	}
};

class B : public A {
public:
	B() {
		cout << "gou zao B" << endl;
	}
	~B() {
		cout << "xi gou B" << endl;
	}
	void print() {
		cout << "class B" << endl;
	}
};

int main(int argc, char **argv)
{
	A* a = new B();
	a->print();
	delete a;

	system("pause");
	return 0;
}
