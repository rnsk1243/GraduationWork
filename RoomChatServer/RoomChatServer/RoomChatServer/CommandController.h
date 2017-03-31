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

class CCommandController
{
	CChannelManager& ChannelManager;
	CRoomManager& RoomManager;
	CChannelHandler& ChannelHandler;
	CRoomHandler& RoomHandler;
public:
	CCommandController(CChannelManager& channelManager, CRoomManager& roomManager, CChannelHandler& channelHandler, CRoomHandler& roomHandler);
	~CCommandController();
	// ��� ó�� �Լ�(�� ����� ��)
	int commandHandling(CLink& clientInfo, char* command);
#pragma region get �Լ�
	//CRoomHandlere* getRoomHandlere() { return RoomHandlere; }
	CChannelHandler& getChannelHandler() { return ChannelHandler; }
#pragma endregion
	bool deleteClientSocket(CLink& clientInfo); // �ι� ���� ȣ�� �Ǹ� ����
};