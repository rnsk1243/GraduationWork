#pragma once
#include<iostream>
#include<vector>
#include<stdarg.h>
#include<fstream>
#include"Link.h"
using namespace std;


class CReadHandler
{
	CReadHandler();
	// 유저 카드 라인 가져오기
	bool ReadUserCardLine(const string textFileName, const char* userName, vector<string>& targetTemp);
public:
	static CReadHandler* getInstance();
	CReadHandler(const CReadHandler&) = delete;
	CReadHandler& operator=(const CReadHandler&) = delete;
	~CReadHandler();
	// 파일명, 전부 일치?, 인자몇개, 스트링(찾으려는 문자의 줄수를 리턴)
	int Search(const char* textFileName, bool isFullMatch, int count, ...);
	// 문자열스트링, 나누는기준문자
	vector<string> Parse(const string& str, const char& ch);
	// 유저 카드 읽기
	bool ReadUserCard(CLink* client, const string textFileName);
	//// 스트링id, 스트링password
	//bool CheckIDandPassWord(const string& id, const string& password)
	//{
	//	if (Search(NameMemberInfoTxt, false, 2, id, password))
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}
	//// 스트링id
	//bool CheckOverlapID(const string& id)
	//{
	//	if (Search(NameMemberInfoTxt, false, 1, id))
	//	{
	//		// 중복됨 
	//		return false;
	//	}
	//	else
	//	{
	//		// 사용가능
	//		return true;
	//	}
	//}
	
};
static CReadHandler* ReadHandlerStatic = CReadHandler::getInstance();
