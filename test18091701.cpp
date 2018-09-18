// MySingleton.h
#pragma once
#include <iostream>
#include <string>

using namespace std;

class MySingleton
{
private:
	static MySingleton* mySingleton;
	static string sname;
protected:
	MySingleton(const string &name);
public:
	static MySingleton* getInstance(const string &name);
	virtual void show();
};

// MySingletonA.h
#pragma once
#include "MySingleton.h"

class MySingletonA : public MySingleton
{
	friend class MySingleton;
private:
	MySingletonA(const string &name) : MySingleton(name) {}
public:
	void show() {
		cout << "MySingletonA" << endl;
	}
};

// MySingletonB.h
#pragma once
#include "MySingleton.h"

class MySingletonB: public MySingleton
{
	friend class MySingleton;
private:
	MySingletonB(const string &name) : MySingleton(name) {}
public:
	void show() {
		cout << "MySingletonB" << endl;
	}
};

// MySingleton.cpp
#include "MySingleton.h"
#include "MySingletonA.h"
#include "MySingletonB.h"

MySingleton* MySingleton::mySingleton = nullptr;
string MySingleton::sname = "???";

MySingleton::MySingleton(const string &name) {
	sname = name;
}

MySingleton* MySingleton::getInstance(const string &name) {
	if (mySingleton == nullptr) {
		if (name == "MySingletonA") {
			mySingleton = new MySingletonA(name);
		}
		else if(name == "MySingletonB"){
			mySingleton = new MySingletonB(name);
		}
		else {
			mySingleton = new MySingleton(name);
		}
	}
	return mySingleton;
}

void MySingleton::show() {
	cout << "MySingleton" << endl;
}


// test smaple
#include <iostream>
#include <string>

#include "MySingleton.h"

using namespace std;

int main(int argc, char **argv)
{
	MySingleton *st = MySingleton::getInstance("MySingletonA");
	MySingleton *sp = MySingleton::getInstance("MySingletonB");
	printf("st :: %p\nsp :: %p\n", st, sp);
	st->show();
	sp->show();

	return 0;
}

