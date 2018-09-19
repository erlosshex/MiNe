#include <list>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <string>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int value) {
	std::lock_guard<std::mutex> guard(some_mutex);
	some_list.push_back(value);
}

bool list_contains(int value_to_find) {
	std::lock_guard<std::mutex> guard(some_mutex);
	bool result;
	result = find(some_list.begin(),some_list.end(),value_to_find)!=some_list.end();
	return result;
}

class some_data {
	int a;
	std::string b;
public:
	void do_something() { std::cout << "some_data" << std::endl; }
};

class data_wrapper {
private:
	some_data data;
	std::mutex m;
public:
	template<typename F>
	void process_data(F func) {
		std::lock_guard<std::mutex> l(m);
		func(data);
	}
};

some_data *unprotected;

void malicious_function(some_data &protected_data) {
	unprotected = &protected_data;
}

data_wrapper x;
void foo() {
	x.process_data(malicious_function);
	unprotected->do_something();
}

void print_list(std::string prompt) {
	std::cout << prompt << std::endl;
	for (auto &item : some_list)
		std::cout << item << " ";
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	for (int i = 0; i < 10; ++i)
		some_list.push_back(i);
	print_list("Start : ");
	some_list.push_back(100);
	print_list("First : (push 100)");
	if (list_contains(8))
		std::cout << "find it -- 8" << std::endl;
	else
		std::cout << "No such entry -- 8" << std::endl;
	//
	foo();
	char c;
	std::cin >> c;
	return 0;
}
