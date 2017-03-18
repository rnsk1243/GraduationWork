#pragma once
#include<iostream>
using namespace std;
#include"MyInfo.h"

const int NoneRoom = -1;

class CRoomHandlere
{
public:
	CRoomHandlere();
	~CRoomHandlere();
	// room���� ������ (���������ϴ� ����)
	bool exitRoom(CMyInfo* clientInfo, CLink* link);
	// ���� �ڽ��� ���� ���� ������ �� �����(�� ����, ����� �� ��ȣ) 
	bool makeRoom(CMyInfo* clientInfo, CLink* link, char* roomName);
	// ���� �ڽ��� ���� ���� ������ �� ���� �ϱ�
	bool enterRoom(CMyInfo* clientInfo, CLink* link, int targetRoomNo);
	// �� ���� ���� �Լ�
	char* returnRoomName(char* message);
};

