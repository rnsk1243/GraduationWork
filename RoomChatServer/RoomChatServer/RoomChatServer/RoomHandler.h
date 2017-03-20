#pragma once
#include<iostream>
using namespace std;
#include"Link.h"
#include"RoomChannelManager.h"
const int NoneRoom = -1;

class CRoomHandler
{
public:
	CRoomHandler();
	~CRoomHandler();
	// room���� ������ (���������ϴ� ����)
	bool exitRoom(CLink* clientInfo, CRoomChannelManager* roomChannelManager);
	// ���� �ڽ��� ���� ���� ������ �� �����(�� ����, ����� �� ��ȣ) 
	bool makeRoom(CLink* clientInfo, CRoomChannelManager* roomChannelManager, char* roomName);
	// ���� �ڽ��� ���� ���� ������ �� ���� �ϱ�
	bool enterRoom(CLink* clientInfo, CRoomChannelManager* roomChannelManager, int targetRoomNo);
	// �� ���� ���� �Լ�
	char* returnRoomName(char* message);
};

