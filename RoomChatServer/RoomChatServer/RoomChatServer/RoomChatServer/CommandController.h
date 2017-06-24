#pragma once
#include"ChannelHandler.h"
#include"RoomHandler.h"
#include"ChannelManager.h"
#include"RoomManager.h"
#include"CardManager.h"
class CCommandController
{
	CChannelManager ChannelManager;
	CRoomManager RoomManager;
	CChannelHandler ChannelHandler;
	CRoomHandler RoomHandler;
	CCardManager mCardManager;
	CGaChar mGacharHandler;
	int cardSelect(shared_ptr<CLink> shared_clientInfo, MessageStruct* sendClientMessage);
	int readyCommand(shared_ptr<CLink> shared_clientInfo, CLink*& clientInfo, int& channelNum);
	int enterRoom(shared_ptr<CLink> shared_clientInfo);
	int changeChannel(shared_ptr<CLink> shared_clientInfo);
	int makeRoom(char * command, shared_ptr<CLink> shared_clientInfo);
	int outRoom(shared_ptr<CLink> shared_clientInfo);
	int mergeRoom(shared_ptr<CLink> shared_clientInfo);
	// 명령 처리 함수(방 만들기 등)
public:
	CCommandController();
	CCommandController(const CCommandController&) = delete;
	CCommandController& operator=(const CCommandController&) = delete;
	~CCommandController();
	int commandHandling(shared_ptr<CLink> shared_clientInfo, char* command, MessageStruct* sendClientMessage);
#pragma region get 함수
	//CRoomHandlere* getRoomHandlere() { return RoomHandlere; }
	CChannelHandler& getChannelHandler() { return ChannelHandler; }
	CChannelManager& getChannelManager() { return ChannelManager; }
	CRoomManager& getRoomManager() { return RoomManager; }
#pragma endregion
	bool deleteClientSocket(CLink& clientInfo); // 두번 연속 호출 되면 오류
};