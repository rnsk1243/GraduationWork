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
	//int BeginToTargetUserLineCurserMoveSize(const char * textName, int targetUserLineNum, char * targetLine);
	//int TargetLineToUserCurserMoveSize(const char * targetSouce, string & searchCardNum, bool & isNewCard, int & cardAmount);
	void WriteCard(const char * textName, int offset, int cardNum, int cardAmount);
	bool writeNextJoinUserNum(const string& textFileName,const int& nextUserNum);
};
static CWriteHandler* WriteHandlerStatic = CWriteHandler::getInstance();

