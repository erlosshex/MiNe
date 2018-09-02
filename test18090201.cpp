// this program will read the context of the configure file and save the copy in a map object,
// with the map object, we can change the context of the map object, 
// in the end, we will write the map object into the previous configure file.
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <iterator>
#include <utility>

// *readFromFile
//  read the context from the 'filePath' configure file, 
//  and copy the context into a string object,
// *input argments
//  -filePath
//   the string object of the string of the configure file,
// *output type
//  -std::string
//   the type of the return object, two conditions,
//   the one is empty string object, it represents failure,
//   the another is a string containing something, it represents success.
std::string readFromFile(const std::string &filePath);

// *str2Map
//  convert a line string into a map object, with the delimit char,
// *input argments
//  -srcStr
//   the string to deal with,
//  -dstMap
//   the map object to save the string,
//  -delimit
//   the char object to seperate the string into two parts,
// *output type
//  -int
//   to represent failure or success, 0 is failure, 1 is success.
int str2Map(const std::string &srcStr, std::map<std::string, std::string> &dstMap, char delimit);

// *lineStr2Map
//  convert multiple-lines string into a map object, with the delimit char;
// *input argments
//  -srcStr
//   the multiple-lines string, which is dealed with,
//  -dstMap
//   the map object to save the multiple-lines string,
//  -delimit
//   the char object to seperate a-line string into two parts,
// *output type
//  -int
//   to represent failure or success, 0 is failure, 1 is success.
int lineStr2Map(const std::string &srcStr, std::map<std::string, std::string> &dstMap, char delimit);

// *saveMap2File
//  save the concerned map object in the concerned file,
// *input argments
//  -srcMap
//   the map object to save,
//  -filePath
//   the name of save file,
// *output type
//  -int
//   to represent failure or success, 0 is failure, 1 is success.
int saveMap2File(const std::map<std::string, std::string> &srcMap, const std::string &filePath);

int main(int argc, char **argv)
{
	std::string path("D:\\Codes\\VS\\VC\\DoCpp\\x64\\Debug\\readFile.txt");
	std::string res = readFromFile(path);
	if (res.empty())  std::cout << "read fail" << std::endl;
	else std::cout << res << std::endl;
	std::map < std::string, std::string > m;
	lineStr2Map(res, m, '=');
	for (auto beg = m.begin(); beg != m.end(); ++beg) {
		std::cout << "*** " << std::to_string((beg->first).length()) << beg->first << " : " << std::to_string((beg->second).length()) << beg->second << std::endl;
	}
	m["thirdLine"] = "ZZZZZZZZ";
	saveMap2File(m,path);
	//
	char c;
	std::cin >> c;
	return 0;
}

// *readFromFile
//  read the context from the 'filePath' configure file, 
//  and copy the context into a string object,
// *input argments
//  -filePath
//   the string object of the string of the configure file,
// *output type
//  -std::string
//   the type of the return object, two conditions,
//   the one is empty string object, it represents failure,
//   the another is a string containing something, it represents success.
std::string readFromFile(const std::string &filePath) {
	std::fstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open())  return std::string();
	std::istreambuf_iterator<char> beg(fileStream), end;
	std::string allContext(beg, end);
	fileStream.close();
	return allContext;
}

// *str2Map
//  convert a line string into a map object, with the delimit char,
// *input argments
//  -srcStr
//   the string to deal with,
//  -dstMap
//   the map object to save the string,
//  -delimit
//   the char object to seperate the string into two parts,
// *output type
//  -int
//   to represent failure or success, 0 is failure, 1 is success.
int str2Map(const std::string &srcStr, std::map<std::string, std::string> &dstMap, char delimit) {
	std::string keyStr, valueStr;
	std::string::size_type posDelimit = srcStr.find(delimit);
	if (posDelimit == std::string::npos)  return 0;
	keyStr = srcStr.substr(0, posDelimit);
	valueStr = srcStr.substr(posDelimit + 1);
	dstMap.insert(std::pair<std::string,std::string>(keyStr,valueStr));
	return 1;
}

// *lineStr2Map
//  convert multiple-lines string into a map object, with the delimit char;
// *input argments
//  -srcStr
//   the multiple-lines string, which is dealed with,
//  -dstMap
//   the map object to save the multiple-lines string,
//  -delimit
//   the char object to seperate a-line string into two parts,
// *output type
//  -int
//   to represent failure or success, 0 is failure, 1 is success.
int lineStr2Map(const std::string &srcStr, std::map<std::string, std::string> &dstMap, char delimit) {
	int res = 0;
	std::stringstream srcStrStream(srcStr,std::ios::in);
	std::string dealStr;
	std::getline(srcStrStream,dealStr);
	while (!dealStr.empty()) {
		res = str2Map(dealStr, dstMap, delimit);
		dealStr = std::string();
		std::getline(srcStrStream,dealStr);
	}
	return res;
}

// *saveMap2File
//  save the concerned map object in the concerned file,
// *input argments
//  -srcMap
//   the map object to save,
//  -filePath
//   the name of save file,
// *output type
//  -int
//   to represent failure or success, 0 is failure, 1 is success.
int saveMap2File(const std::map<std::string, std::string> &srcMap, const std::string &filePath) {
	std::string saveStr;
	auto beg = srcMap.cbegin();
	auto end = srcMap.cend();
	for (auto iter = beg; iter != end; ++iter)
		saveStr = saveStr + iter->first + "=" + iter->second + "\n";
	std::fstream fileStream(filePath, std::ios::out|std::ios::trunc);
	if (!fileStream.is_open())  return 0;
	fileStream << saveStr;
	return 1;
}
