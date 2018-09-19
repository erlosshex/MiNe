#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv)
{
	vector<int> iv{ 1,2,3,4,5 };
	auto first = iv.begin();
	auto second = first;
	second = second + 3;
	auto third = second + 1;
	for (auto i = 0; i < iv.size(); ++i) {
		printf("%d : %d\n", i, iv.at(i));
	}
	{
		// address ip
		int i = 1;
		cout << "----------------------------------" << endl;
		cout << "size : " << iv.size() << " -- capacity : " << iv.capacity() << endl;
		for (auto tmp = iv.begin(); tmp != iv.end(); ++tmp, ++i)
			printf("address %d : %p : %d\n", i, &tmp,*tmp);
		cout << endl;
		cout << "----------------------------------" << endl;
		//
	}
	
	printf("first address : %p\n", &iv);
	third = iv.emplace(second, 66);
	printf("******************\n");
	first = iv.begin();
	printf("11 : %d\n", *first);
	printf("22 : %d\n", *(third+2));
	printf("second address : %p\n",&iv);
	{
		// address ip
		int i = 1;
		cout << "----------------------------------" << endl;
		cout << "size : " << iv.size() << " -- capacity : " << iv.capacity() << endl;
		for (auto tmp = iv.begin(); tmp != iv.end(); ++tmp, ++i)
			printf("address %d : %p : %d\n", i, &tmp,*tmp);
		cout << endl;
		cout << "----------------------------------" << endl;
		//
	}
	//
	vector<int> iiv;
	auto begin2 = iiv.begin();
	for (auto begin1 = iv.begin(); begin1 != iv.end(); ++begin1) {
		begin2 = iiv.insert(begin2, *begin1);
		begin2++;
	}
	for (auto b1 = iiv.begin(); b1 != iiv.end(); ++b1)
		printf("%d ",*b1);
	cout << endl;
	///
	{
		vector<int> iv{ 0,1,2,3,4,5,6,7,8,9 };
		for (int i = 0; i < iv.size(); ++i)
			cout << iv.at(i) << " ";
		cout << endl;
		auto iter = iv.begin();
		while (iter != iv.end()) {
			if ((*iter) % 2 == 0) {
				iter = iv.insert(iter, *iter);
				iter += 2;
			}
			else {
				iter = iv.erase(iter);
			}
		}
		for (int i = 0; i < iv.size(); ++i)
			cout << iv.at(i) << " ";
		cout << endl;
	}
	
	///
	//
	char c;
	cin >> c;

	return 0;
}
