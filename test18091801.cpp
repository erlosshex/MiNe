#include <iostream>
#include <string>

using namespace std;

class Phone {
public:
	Phone() {}
	virtual ~Phone() {}
	virtual void showDecorate() {}
};

class IPhone : public Phone {
private:
	string m_name;
public:
	IPhone(const string &name) : m_name(name) {}
	~IPhone() {}
	void showDecorate() {
		cout << m_name << "'s descriptor" << endl;
	}

};

class NokiaPhone : public Phone {
private:
	string m_name;
public:
	NokiaPhone(const string &name) : m_name(name) {}
	~NokiaPhone() {}
	void showDecorate() {
		cout << m_name << "'s descriptor" << endl;
	}
};

class DecoratorPhone : public Phone {
private:
	// this is important, descriptor
	Phone *m_phone;
public:
	DecoratorPhone(Phone *phone) : m_phone(phone) {}
	virtual void showDecorate() {
		m_phone->showDecorate();
	}
};

class DecoratorPhoneA : public DecoratorPhone {
public:
	DecoratorPhoneA(Phone *phone) : DecoratorPhone(phone) {}
	void showDecorate() {
		addDecorate();
	}
private:
	void addDecorate() {
		cout << "add another thing" << endl;
	}
};

class DecoratorPhoneB : public DecoratorPhone {
public:
	DecoratorPhoneB(Phone *phone) : DecoratorPhone(phone) {}
	void showDecorate() {
		DecoratorPhone::showDecorate();
		addDecorate();
	}
private:
	void addDecorate() {
		cout << "ping mu tie mo" << endl;
	}
};

// test code
int main(int argc, char **argv)
{
	Phone *iphone = new NokiaPhone("6300");
	Phone *dpa = new DecoratorPhoneA(iphone);
	Phone *dpb = new DecoratorPhoneB(dpa);

	dpb->showDecorate();
	delete dpa;
	delete dpb;
	delete iphone;
	return 0;
}
