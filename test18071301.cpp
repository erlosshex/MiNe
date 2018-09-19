#include <iostream>
#include <limits.h>

#define f(x) ((x)-1)
#define abs(x) ((x)>0?(x):(-(x)))

char* hello() {
	return __func__;
}

char* world() {
	return __func__;
}

int main(int argc, char *argv[])
{
	int x = 1;
	std::cout << "first : " << f(3) << std::endl;
	std::cout << "second : " << f(3) << std::endl;
	std::cout << "third : " << abs(4) << std::endl;
	std::cout << "fouth : " << abs(-4) << std::endl;
	std::cout << "fifth : " << abs(-6) + 2 << std::endl;
	int y = (x,(-x)-1,5);
	std::cout << "sixth : " << y << std::endl;
	//std::cout << "int max : " << MAX_INT << std::endl;
	std::cout << "4/3 : " << 4 / 3 << std::endl;
	std::cout << "(-4)/3 : " << (-4) / 3 <<" | (-4)%3 : "<< (-4) % 3 << std::endl;
	std::cout << "4/(-3) : " << 4 / (-3) <<" | 4/(-3) : "<< 4 % (-3) << std::endl;
	//
	std::cout << hello() << ", " << world() << std::endl;
	//std::cout << "stdc : " << __STDC__ << std::endl;
	char c;
	std::cin >> c;

	return 0;
}
