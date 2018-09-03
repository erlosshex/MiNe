#include <iostream>
#include <thread>

int num_global = 0;

void hello() {
	std::cout << "hello concurrent world" << std::endl;
}

void do_something() {
	std::cout << "do something" << std::endl;
}

void do_something_else() {
	std::cout << "do something else" << std::endl;
}

class background_task
{
public:
	mutable int num_current;
	background_task(int n = 0) :num_current(n) {}
	void operator()() const {
		std::cout << "thread num_current id : " << &num_current << std::endl;
		do_something();
		do_something_else();
		num_current = 5;
		num_global = 5;
	}
};

void testFunc1() {
	background_task  test1Bt1(24);
	std::thread t1(test1Bt1);
	std::thread t2(test1Bt1);
	t1.detach();
	t2.detach();
}

int main(int argc, char **argv)
{
	testFunc1();
	//
	num_global = 20;
	std::cout << "num_global -> " << num_global << std::endl;
	//
	std::thread t1(hello);
	t1.join();
	//
	background_task bt2(10);
	std::cout << "before num_current id : " << &bt2.num_current << std::endl;
	std::thread t2(bt2);
	t2.join();
	//
	std::cout << "after num_current id : " << &bt2.num_current << std::endl;
	std::cout << "bt2 -> " << bt2.num_current << std::endl;
	std::cout << "num_global -> " << num_global << std::endl;
	//
	char c;
	std::cin >> c;
	return 0;
}
