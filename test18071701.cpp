#include <iostream>
#include <thread>

struct myFunc {
public:
	myFunc(int n1) :n(n1) {}
	void operator()() {
		std::cout << "hello world : " << n << std::endl;
	}
private:
	int n;
};


class myFunc1 {
public:
	myFunc1():n(5){}
	myFunc1(int n1):n(n1){}
	void operator()(int n2, int &result) {
		std::cout << "inner id : " << std::this_thread::get_id() << std::endl;
		result = n + n2;
	}
private:
	int n;
};

int main(int argc, char **argv)
{
	int result = 0;
	//
	myFunc f1(5);
	std::thread t1(f1);
	t1.join();
	//
	myFunc1 ff1;
	std::thread t2{ ff1,5,std::ref(result) };
	std::cout << "ff1 : " << result << std::endl;
	std::cout << "t2 outer id : " << t2.get_id() << std::endl;
	t2.join();
	//
	myFunc1 ff2{ 45 };
	std::thread t3{ ff2,5,std::ref(result) };
	std::cout << "ff2 : " << result << std::endl;
	t3.join();
	//
	std::thread t4{ myFunc1(32),7,std::ref(result) };
	std::cout << "t4 : " << result << std::endl;
	t4.join();
	//
	std::thread t5{ myFunc1{312}, 7, std::ref(result)};
	std::cout << "t5 : " << result << std::endl;
	t5.join();
	//
	char c;
	std::cin >> c;
	return 0;
}
