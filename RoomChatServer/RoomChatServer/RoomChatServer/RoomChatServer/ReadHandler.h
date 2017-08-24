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
	bool ReadUserCard(const LinkPtr& client, const string& textFileName);
	// 유저 재화 읽기
	bool ReadUserGoods(const LinkPtr& client, const string& textFileName);
	// 다음 유저 번호 가져오기
	const string GetNextUserNum(const string& textFileName);
};
static CReadHandler* ReadHandlerStatic = CReadHandler::GetInstance();
