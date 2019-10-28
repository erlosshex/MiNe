#pragma once
#include <io.h>
#include <vector>
#include <string>

using namespace std;

void getAllFiles(string path, vector<string>& files);
bool getFileNameVecFromDir(const std::string &dirPath, std::vector<std::string> &fileNameVec, std::vector<std::string> &dirNameVec);
