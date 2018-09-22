// static attribution of class
#include <iostream>
#include <string>

using namespace std;

class Base {
public:
	Base() : name("") {
		count++;
	};
	Base(const string &s): name(s) {
		count++;
	}
	static void print() {
		cout << "Base : " << count << endl;
	}
	~Base() {
		count--;
	}
private:
	string name;
	static int count;
};

int Base::count = 0;

class Derive : public Base {
public:
	Derive(int n) : num(n) {}
	Derive(const string &s, int n) : Base(s), num(n) {}
private:
	int num;
};

int main(int argc, char **argv)
{
	Base::print();
	Derive d1(1);
	Base::print();
	Derive d2("hello", 2);
	Base::print();
	Derive *d3 = new Derive("world", 3);
	Base::print();
	delete d3;
	Base::print();
	//
	char c;
	cin >> c;
	return 0;
}
