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
	static CWriteHandler* GetInstance();
	// 파일명, 몇개, 스트링
	bool Write(const char* textFileName, int count, ...);
	//int writeGacharResult(const char* textFileName, const char* userName);
	//int BeginToTargetUserLineCurserMoveSize(const char * textName, int targetUserLineNum, char * targetLine);
	//int TargetLineToUserCurserMoveSize(const char * targetSouce, string & searchCardNum, bool & isNewCard, int & cardAmount);
	bool WriteObj(const string& textName, int offset, int recordedNumber, int cipher);
	bool WriteNextJoinUserNum(const string& textFileName,const int& nextUserNum);
};
static CWriteHandler* WriteHandlerStatic = CWriteHandler::GetInstance();

