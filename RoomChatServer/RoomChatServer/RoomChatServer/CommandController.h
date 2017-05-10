#pragma once
#include"ChannelHandler.h"
#include"RoomHandler.h"
#include"ChannelManager.h"
#include"RoomManager.h"

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