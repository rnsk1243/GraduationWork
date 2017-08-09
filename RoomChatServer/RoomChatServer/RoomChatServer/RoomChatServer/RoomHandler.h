#pragma once
#include<iostream>
using namespace std;
#include"Link.h"
#include"RoomManager.h"


class CRoomHandler
{
	bool GetMyRoomIter(CLink* clientInfo, CRoomManager* roomManager, RoomListIt& resultRoomIter);
public:
	CRoomHandler(const CRoomHandler&) = delete;
	CRoomHandler& operator=(const CRoomHandler&) = delete;
	CRoomHandler();
	~CRoomHandler();
	// room���� ������ (���������ϴ� ����)
	bool ExitRoom(CLink* clientInfo, CRoomManager* roomManager);
	// ���� �ڽ��� ���� ���� ������ �� �����(�� ����, ����� �� ��ȣ) 
	bool MakeRoom(const shared_ptr<CLink>& shared_clientInfo, CRoomManager* roomManager, const string& roomName,const int& battingMoney);
	// ���� �ڽ��� ���� ���� ������ �� ���� �ϱ�
	bool EnterRoom(const shared_ptr<CLink>& shared_clientInfo, CRoomManager* roomManager, int targetRoomNo);
	// �� ���� ���� �Լ�
	//char* returnRoomName(char* message);
	// �濡 �ִ� ��� ���� ���� �غ� �Ƴ�?
	bool IsAllReadyGame(CLink* clientInfo, CRoomManager* roomManager);
	// ���� ��� ������ �غ� �Ƴ�?
	bool IsAllReadyBatting(CLink* clientInfo, CRoomManager* roomManager);
};

