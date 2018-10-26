#include <iostream>

using namespace std;

class Base {
public:
	virtual void print() {
		cout << "This is Base." << endl;
	}
};

class Derive : public Base {
public:
	virtual void print() {
		cout << "This is Derive." << endl;
	}
};

int main(int argc, char **argv)
{
	Base *pb = new Derive();
	Base &rb = *pb;
	Base b = *pb;
	pb->print();       // : This is Derive.
	rb.print();        // : This is Derive.
	b.print();         // : This is Base.
	//
	char c;
	cin >> c;
	return 0;
}
