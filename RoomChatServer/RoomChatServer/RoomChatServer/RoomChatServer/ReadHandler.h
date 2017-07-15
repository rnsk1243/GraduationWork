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
	// ���� ��ü ���� ��������
	bool ReadUserObjectLine(const string& textFileName, const int& userPKNum, vector<string>& targetTemp);
	// ������ ���� ��������
	const string GetLastLine(const string& textFileName);
public:
	static CReadHandler* GetInstance();
	CReadHandler(const CReadHandler&) = delete;
	CReadHandler& operator=(const CReadHandler&) = delete;
	~CReadHandler();
	// ���ϸ�, ������, ���ڸ, ��Ʈ��(ã������ ������ �ټ��� ����)
	int Search(const char* textFileName, vector<string>& tempUserInfo, int count, ...);
	// ���ڿ���Ʈ��, �����±��ع���
	vector<string> Parse(const string& str, const char& ch);
	// ���� ī�� �б�
	bool ReadUserCard(CLink* client, const string& textFileName);
	// ���� ��ȭ �б�
	bool ReadUserGoods(CLink* client, const string& textFileName);
	// ���� ���� ��ȣ ��������
	const string GetNextUserNum(const string& textFileName);
	//// ��Ʈ��id, ��Ʈ��password
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
	//// ��Ʈ��id
	//bool CheckOverlapID(const string& id)
	//{
	//	if (Search(NameMemberInfoTxt, 1, id))
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
static CReadHandler* ReadHandlerStatic = CReadHandler::GetInstance();
