#pragma once
#include<iostream>
#include<vector>
#include<stdarg.h>
#include<fstream>
using namespace std;


class CReadHandler
{
public:
	CReadHandler();
	CReadHandler(const CReadHandler&) = delete;
	CReadHandler& operator=(const CReadHandler&) = delete;
	~CReadHandler();
	// 파일명, 전부 일치?, 인자몇개, 스트링
	bool Search(const char* textFileName, bool isFullMatch, int count, ...);
	// 문자열스트링, 나누는기준문자
	vector<string> Parse(const string& str, const char& ch);
	// 스트링id, 스트링password
	bool CheckIDandPassWord(const string& id, const string& password)
	{
		if (Search("test.txt", false, 2, id, password))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// 스트링id
	bool CheckOverlapID(const string& id)
	{
		if (Search("test.txt", false, 1, id))
		{
			// 중복됨 
			return false;
		}
		else
		{
			// 사용가능
			return true;
		}
	}
};

