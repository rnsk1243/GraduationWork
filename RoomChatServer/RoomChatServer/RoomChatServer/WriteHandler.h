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
	// ���ϸ�, �, ��Ʈ��
	bool write(const char* textFileName, int count, ...);
};

