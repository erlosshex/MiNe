// this program tests the signal process and preprocess function in C/C++
#include <windows.h>
#include <string>

#define MKSTR(x,y) (x##y)
#define TEST(x) (#x)
#define TEST1(x) (x)

const std::string x = "hello";
const std::string xy = "hello,world";

void signalHandler(int signum) {
	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
	std::cout << "TEST(x) --> " << TEST(x) << std::endl;
	std::cout << "TEST1(x) --> " << TEST1(x) << std::endl;
	std::cout << "TEST(\"x\") --> " << TEST("x") << std::endl;
	std::cout << "TEST1(\"x\") --> " << TEST1("x") << std::endl;
	std::cout << "MKSTR(x,y) --> " << MKSTR(x, y) << std::endl;
	std::cout << "MKSTR(\"x\",\"y\") --> " << MKSTR("x", "y") << std::endl;
	system("pause");
	exit(signum);
}

int main(int argc, char **argv)
{
	int i = 0;

	std::signal(SIGINT, signalHandler);

	while (++i) {
		std::cout << "Going to sleep ..." << std::endl;
		if (10 == i)  std::raise(SIGINT);
		Sleep(500);
	}
	return 0;
}
