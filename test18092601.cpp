#include <iostream>

using namespace std;

class Resume {
public:
	void fillResume() {
		setPersonInfo();
		setEducation();
		setWorkExp();
	}
protected:
	//virtual void setPersonInfo() = 0;
	//virtual void setEducation() = 0;
	//virtual void setWorkExp() = 0;
	virtual void setPersonInfo() {
		cout << "setPersonInfo ..." << endl;
	}
	virtual void setEducation() {
		cout << "setEducation ..." << endl;
	}
	virtual void setWorkExp() {
		cout << "setWorkExp ..." << endl;
	}
};

class ResumeA : public Resume {
protected:
	virtual void setPersonInfo() {
		cout << "A -- setPersonInfo ..." << endl;
	}
	virtual void setEducation() {
		cout << "A -- setEducation ..." << endl;
	}
	virtual void setWorkExp() {
		cout << "A -- setWorkExp ..." << endl;
	}
};

class ResumeB : public Resume {
protected:
	virtual void setPersonInfo() {
		cout << "B -- setPersonInfo ..." << endl;
	}
	virtual void setEducation() {
		cout << "B -- setEducation ..." << endl;
	}
	virtual void setWorkExp() {
		cout << "B -- setWorkExp ..." << endl;
	}
};

int main(int argc, char **argv)
{
	Resume *p1 = new ResumeA;
	Resume *p2 = new ResumeB;
	p1->fillResume();
	p2->fillResume();

	//
	char c;
	cin >> c;
	return 0;
}
