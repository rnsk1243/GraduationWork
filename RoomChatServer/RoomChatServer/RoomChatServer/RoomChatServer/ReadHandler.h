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
	// ���� ī�� ���� ��������
	bool ReadUserCardLine(const string textFileName, const char* userName, vector<string>& targetTemp);
public:
	static CReadHandler* getInstance();
	CReadHandler(const CReadHandler&) = delete;
	CReadHandler& operator=(const CReadHandler&) = delete;
	~CReadHandler();
	// ���ϸ�, ���� ��ġ?, ���ڸ, ��Ʈ��(ã������ ������ �ټ��� ����)
	int Search(const char* textFileName, bool isFullMatch, int count, ...);
	// ���ڿ���Ʈ��, �����±��ع���
	vector<string> Parse(const string& str, const char& ch);
	// ���� ī�� �б�
	bool ReadUserCard(CLink* client, const string textFileName);
	//// ��Ʈ��id, ��Ʈ��password
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
	//// ��Ʈ��id
	//bool CheckOverlapID(const string& id)
	//{
	//	if (Search(NameMemberInfoTxt, false, 1, id))
	//	{
	//		// �ߺ��� 
	//		return false;
	//	}
	//	else
	//	{
	//		// ��밡��
	//		return true;
	//	}
	//}
	
};
static CReadHandler* ReadHandlerStatic = CReadHandler::getInstance();
