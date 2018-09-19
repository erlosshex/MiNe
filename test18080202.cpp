#include <iostream>

int main(int argc, char** argv)
{
	try 
	{
		std::string s = "hello";
		std::string s1 = s.substr(1,4);
		std::cout << s.c_str() << std::endl;
	}
	catch(const std::exception &e)
	{
		std::cout << "message : " << e.what() << std::endl;
	}
	char c;
	std::cin >> c;
	return 0;
}
