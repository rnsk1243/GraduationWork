#pragma once
#include<iostream>
using namespace std;
#include"Link.h"
#include"RoomManager.h"
const int NoneRoom = -1;

class CRoomHandler
{
public:
	CRoomHandler();
	~CRoomHandler();
	// room���� ������ (���������ϴ� ����)
	bool exitRoom(CLink* clientInfo, CRoomManager* roomManager);
	// ���� �ڽ��� ���� ���� ������ �� �����(�� ����, ����� �� ��ȣ) 
	bool makeRoom(CLink* clientInfo, CRoomManager* roomManager, char* roomName);
	// ���� �ڽ��� ���� ���� ������ �� ���� �ϱ�
	bool enterRoom(CLink* clientInfo, CRoomManager* roomManager, int targetRoomNo);
	// �� ���� ���� �Լ�
	char* returnRoomName(char* message);
};

