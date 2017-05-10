#pragma once
#include<iostream>
#include<vector>
#include<stdarg.h>
#include<fstream>
using namespace std;

class CWriteHandler
{
public:
	CWriteHandler();
	~CWriteHandler();
	// 파일명, 몇개, 스트링
	bool write(const char* textFileName, int count, ...);
};

