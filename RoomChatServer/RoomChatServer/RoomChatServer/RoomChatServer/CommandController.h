#pragma once
#include"ChannelHandler.h"
#include"RoomHandler.h"
#include"ChannelManager.h"
#include"RoomManager.h"
#include"CardManager.h"
class CCommandController
{
	CChannelManager mChannelManager;
	CRoomManager mRoomManager;
	CChannelHandler mChannelHandler;
	CRoomHandler mRoomHandler;
	CCardManager mCardManager;
	int CardSelect(shared_ptr<CLink> shared_clientInfo, MessageStruct* sendClientMessage);
	int ReadyCommand(shared_ptr<CLink> shared_clientInfo, CLink*& clientInfo, int& channelNum);
	int EnterRoom(shared_ptr<CLink> shared_clientInfo);
	int ChangeChannel(shared_ptr<CLink> shared_clientInfo);
	int MakeRoom(const string& roomName, shared_ptr<CLink> shared_clientInfo);
	int OutRoom(shared_ptr<CLink> shared_clientInfo);
	int MergeRoom(shared_ptr<CLink> shared_clientInfo);
	int CardCompose(shared_ptr<CLink> shared_clientInfo,const string& targetCardNum , const string& sourceCardNum, MessageStruct* sendClientMessage);
	int CardEvolution(shared_ptr<CLink> shared_clientInfo, const string& targetCardNum, MessageStruct* sendClientMessage);
	// 명령 처리 함수(방 만들기 등)
public:
	CCommandController();
	CCommandController(const CCommandController&) = delete;
	CCommandController& operator=(const CCommandController&) = delete;
	~CCommandController();
	int CommandHandling(shared_ptr<CLink> shared_clientInfo, vector<string>& commandString, MessageStruct* sendClientMessage);
#pragma region get 함수
	//CRoomHandlere* getRoomHandlere() { return RoomHandlere; }
	CChannelHandler& GetChannelHandler() { return mChannelHandler; }
	CChannelManager& GetChannelManager() { return mChannelManager; }
	CRoomManager& GetRoomManager() { return mRoomManager; }
#pragma endregion
	bool DeleteClientSocket(CLink& clientInfo); // 두번 연속 호출 되면 오류
};