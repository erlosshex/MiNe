#include <iostream>
#include <string>

using namespace std;

class BaseA {
private:
	string m_name;
public:
	BaseA(): m_name("null") {}
	BaseA(const string &name) :m_name(name) {}
	~BaseA() {
		cout << "This is BaseA destroy" << endl;
	}
	virtual void show() {
		cout << "BaseA : " << m_name << endl;
	}
};

class DeriveA : public BaseA {
private:
	string m_name;
public:
	DeriveA(const string &name) : BaseA("AAAA"),m_name(name) {}
	~DeriveA() {
		cout << "This is DeriveA destroy" << endl;
	}
	virtual void show() {
		cout << "DeriveA : " << m_name << endl;
	}
};

class BaseB {
private:
	string m_name;
public:
	BaseB() : m_name("null") {}
	BaseB(const string &name) :m_name(name) {}
	virtual ~BaseB() {
		cout << "This is BaseB destroy" << endl;
	}
	virtual void show() {
		cout << "BaseB : " << m_name << endl;
	}
};

class DeriveB : public BaseB {
private:
	string m_name;
public:
	DeriveB(const string &name) : BaseB("BBBB"),m_name(name) {}
	virtual ~DeriveB() {
		cout << "This is DeriveB destroy" << endl;
	}
	virtual void show() {
		cout << "DeriveB : " << m_name << endl;
	}
};

int main(int argc, char **argv)
{
	BaseA *pa = new DeriveA("hello");
	BaseB *pb = new DeriveB("world");
	pa->show();
	pb->show();
	delete pa;
	delete pb;
	pa = nullptr;
	pb = nullptr;
	//
	char c;
	cin >> c;
	return 0;
}
