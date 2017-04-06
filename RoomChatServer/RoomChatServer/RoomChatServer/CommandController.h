#pragma once
#include"ChannelHandler.h"
#include"RoomHandler.h"
#include"ChannelManager.h"
#include"RoomManager.h"

const int IntSize = 4;
const int OccuredError = -1;
const int SuccesRecv = 1;
const int SuccesCommand = 2;
const int SuccesSend = 3;
const int EnterRoomPeopleLimit = 2;
const int MaxChannelNum = 5;
const int EnterChannelNum = 1; // ó�� ���� ä��
const int ChannelAmount = 5;

class CCommandController
{
	CChannelManager ChannelManager;
	CRoomManager RoomManager;
	CChannelHandler ChannelHandler;
	CRoomHandler RoomHandler;
	CCommandController(const CCommandController&);
	CCommandController& operator=(const CCommandController&);
public:
	CCommandController();
	~CCommandController();
	// ��� ó�� �Լ�(�� ����� ��)
	int commandHandling(CLink& clientInfo, char* command);
#pragma region get �Լ�
	//CRoomHandlere* getRoomHandlere() { return RoomHandlere; }
	CChannelHandler& getChannelHandler() { return ChannelHandler; }
	CChannelManager& getChannelManager() { return ChannelManager; }
	CRoomManager& getRoomManager() { return RoomManager; }
#pragma endregion
	bool deleteClientSocket(CLink& clientInfo); // �ι� ���� ȣ�� �Ǹ� ����
};