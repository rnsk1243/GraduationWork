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
	// ���ϸ�, ���� ��ġ?, ���ڸ, ��Ʈ��
	bool Search(const char* textFileName, bool isFullMatch, int count, ...);
	// ���ڿ���Ʈ��, �����±��ع���
	vector<string> Parse(const string& str, const char& ch);
	// ��Ʈ��id, ��Ʈ��password
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
	// ��Ʈ��id
	bool CheckOverlapID(const string& id)
	{
		if (Search("test.txt", false, 1, id))
		{
			// �ߺ��� 
			return false;
		}
		else
		{
			// ��밡��
			return true;
		}
	}
};

