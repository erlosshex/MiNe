#include "utility.h"

void getAllFiles(string path, vector<string>& files) {
	//文件句柄
	long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;  //很少用的文件信息读取结构
	string p;  //string类很有意思的一个赋值函数:assign()，有很多重载版本
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib & _A_SUBDIR)) {  //比较文件类型是否是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else {
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}

bool getFileNameVecFromDir(const std::string &dirPath, std::vector<std::string> &fileNameVec, std::vector<std::string> &dirNameVec) {
	bool res = false;

	try {
		fileNameVec.clear();
		dirNameVec.clear();
		intptr_t hFile = 0;
		_finddata_t fileInfo;
		std::string pathName, exdName;
		if ((hFile = _findfirst(pathName.assign(dirPath).append("\\*").c_str(), &fileInfo)) == -1) {
			return false;
		}
		do {
			//std::cout << fileInfo.name << (fileInfo.attrib & _A_SUBDIR ? "[folder]" : "[file]") << std::endl;
			std::string fileInfoName(fileInfo.name);
			if (fileInfo.attrib & _A_SUBDIR) {
				if (fileInfoName != "." && fileInfoName != "..") {
					dirNameVec.push_back(fileInfoName);
				}
			}
			else {
				fileNameVec.push_back(fileInfoName);
			}
		} while ( _findnext(hFile, &fileInfo)== 0);
		_findclose(hFile);
		res = true;
	}
	catch (std::exception &e) {
		res = false;
	}

	return res;
}
