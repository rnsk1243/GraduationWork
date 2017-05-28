#pragma once
#include<iostream>
#include<vector>
#include<stdarg.h>
#include<fstream>
using namespace std;

class CWriteHandler
{
	CWriteHandler();
public:
	CWriteHandler(const CWriteHandler&) = delete;
	CWriteHandler& operator=(const CWriteHandler&) = delete;
	~CWriteHandler();
	static CWriteHandler* getInstance();
	// ���ϸ�, �, ��Ʈ��
	bool write(const char* textFileName, int count, ...);
	//int writeGacharResult(const char* textFileName, const char* userName);
};
static CWriteHandler* WriteHandlerStatic = CWriteHandler::getInstance();

