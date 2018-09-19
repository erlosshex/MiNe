#include <iostream>
#include <thread>
#include <mutex>
#include <stack>
#include <vector>

std::stack<std::vector<int>> some_stack;

void func() {
	for (int i = 0; i < 100; ++i)
		std::cout << "1" << std::endl;
}

void print_stack() {
	for (int i = 0; i < 10; ++i)
		std::cout << "*";
	std::cout << std::endl;
	for (auto &item : some_stack.top()) {
		std::cout << item << " ";
	}
	std::cout << std::endl;
		
}


int main(int argc, char **argv)
{
	some_stack.push({ 0,0,0 });

	some_stack.push({ 1,2,3,4,5 });
	some_stack.push({ 2,3,4,5,6 });
	some_stack.push({ 3,4,5,6,7 });
	some_stack.push({ 4,5,6,7,8 });
	some_stack.push({ 5,6,7,8,9 });

	print_stack();

	for (auto &item : some_stack.top())
		std::cout << "du :" << item << std::endl;
	some_stack.pop();
	print_stack();
	//
	std::thread t1(func);
	for (int i = 0; i < 100; ++i)
		std::cout << "2" << std::endl;
	t1.join();
	//
	std::thread t2;
	std::cout << "t2 id : " << t2.get_id() << std::endl;
	t2 = std::thread(func);
	std::cout << "t2 id2 : " << t2.get_id() << std::endl;
	t2.join();
	//
	char c;
	std::cin >> c;
	return 0;
}
