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
	// 유저 객체 라인 가져오기
	bool ReadUserObjectLine(const string& textFileName, const int& userPKNum, vector<string>& targetTemp);
	// 마지막 라인 가져오기
	const string GetLastLine(const string& textFileName);
public:
	static CReadHandler* GetInstance();
	CReadHandler(const CReadHandler&) = delete;
	CReadHandler& operator=(const CReadHandler&) = delete;
	~CReadHandler();
	// 파일명, 시작점, 인자몇개, 스트링(찾으려는 문자의 줄수를 리턴)
	int Search(const char* textFileName, vector<string>& tempUserInfo, int count, ...);
	// 문자열스트링, 나누는기준문자
	vector<string> Parse(const string& str, const char& ch);
	// 유저 카드 읽기
	bool ReadUserCard(CLink* client, const string& textFileName);
	// 유저 재화 읽기
	bool ReadUserGoods(CLink* client, const string& textFileName);
	// 다음 유저 번호 가져오기
	const string GetNextUserNum(const string& textFileName);
	//// 스트링id, 스트링password
	//bool CheckIDandPassWord(const string& id, const string& password)
	//{
	//	if (Search(NameMemberInfoTxt, 2, id, password))
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
	//	if (Search(NameMemberInfoTxt, 1, id))
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
static CReadHandler* ReadHandlerStatic = CReadHandler::GetInstance();
