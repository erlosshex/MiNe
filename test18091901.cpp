// a simple sample for C++ Observer Pattern of Blog and subscriber
#include <iostream>
#include <list>
#include <string>

using namespace std;

class Observer{
public:
	Observer() {}
	virtual ~Observer() {}
	// ***this is important***
	virtual void update() {}
};

class Blog {
public:
	Blog() {}
	virtual ~Blog() {}
	// add an observer
	void attach(Observer *observer) {
		m_observers.push_back(observer);
	}
	// remove an observer
	void remove(Observer *observer) {
		m_observers.remove(observer);
	}
	// ***this is important***
	void notify() {
		list<Observer*>::iterator iter = m_observers.begin();
		for (; iter != m_observers.end(); ++iter)
			(*iter)->update();
	}
	virtual void setStatus(string s) {
		m_status = s;
	}
	virtual string getStatus() {
		return m_status;
	}
private:
	list<Observer*> m_observers;
protected:
	string m_status;
};

// concrete Blog class
class BlogCSDN : public Blog {
private:
	string m_name;
public:
	BlogCSDN(string name) : m_name(name) {}
	~BlogCSDN() {}
	void setStatus(string s) {
		m_status = "CSDN : " + m_name + " : " + s;
	}
	string getStatus(string s) {
		return m_status;
	}
};

// concrete Observer class
class ObserverBlog : public Observer {
private:
	string m_name;
	Blog *m_blog;
public:
	ObserverBlog(string name, Blog *blog): m_name(name), m_blog(blog) {}
	~ObserverBlog() {}
	void update() {
		string status = m_blog->getStatus();
		cout << m_name << "---" << status << endl;
	}
};

int main(int argc, char **argv)
{
	Blog *blog = new BlogCSDN("erlosshex");
	Observer *observer1 = new ObserverBlog("gu", blog);
	blog->attach(observer1);
	blog->setStatus("Observer Design Pattern");
	blog->notify();
	delete blog;
	delete observer1;
	//
	char c;
	cin >> c;
	return 0;
}
