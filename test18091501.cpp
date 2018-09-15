// test C++ namespace

#include <iostream>
#include <string>

namespace GU {
	int print1() {
		std::cout << "print1 : hello world" << std::endl;
		return 0;
	}
}

namespace GU {
	int print1(const std::string &s);
	int print2(const std::string &s);
}


int main(int argc, char **argv)
{
	//first
	GU::print1();
	
	//second
	//int GU::print1(const std::string &);
	GU::print1("GU");
	//third
	//int GU::print2(const std::string &);
	GU::print2("GU");
	//
	char c;
	std::cin >> c;
	return 0;
}


namespace GU {
	int print1(const std::string &s) {
		std::cout << "print1 : " << s << std::endl;
		return 0;
	}
	int print2(const std::string &s) {
		std::cout << "print2 : " << s << " TEST!" << std::endl;
		return 0;
	}
}
