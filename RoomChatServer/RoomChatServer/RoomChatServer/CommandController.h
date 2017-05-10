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
	// 명령 처리 함수(방 만들기 등)
	int commandHandling(CLink& clientInfo, char* command);
#pragma region get 함수
	//CRoomHandlere* getRoomHandlere() { return RoomHandlere; }
	CChannelHandler& getChannelHandler() { return ChannelHandler; }
	CChannelManager& getChannelManager() { return ChannelManager; }
	CRoomManager& getRoomManager() { return RoomManager; }
#pragma endregion
	bool deleteClientSocket(CLink& clientInfo); // 두번 연속 호출 되면 오류
};