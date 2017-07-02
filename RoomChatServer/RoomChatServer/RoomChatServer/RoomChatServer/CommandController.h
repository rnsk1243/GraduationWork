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
	int cardSelect(shared_ptr<CLink> shared_clientInfo, MessageStruct* sendClientMessage);
	int readyCommand(shared_ptr<CLink> shared_clientInfo, CLink*& clientInfo, int& channelNum);
	int enterRoom(shared_ptr<CLink> shared_clientInfo);
	int changeChannel(shared_ptr<CLink> shared_clientInfo);
	int makeRoom(const string& roomName, shared_ptr<CLink> shared_clientInfo);
	int outRoom(shared_ptr<CLink> shared_clientInfo);
	int mergeRoom(shared_ptr<CLink> shared_clientInfo);
	int cardCompose(shared_ptr<CLink> shared_clientInfo,const string& targetCardNum , const string& sourceCardNum, MessageStruct* sendClientMessage);
	int cardAwake(shared_ptr<CLink> shared_clientInfo, const string& targetCardNum, MessageStruct* sendClientMessage);
	// ��� ó�� �Լ�(�� ����� ��)
public:
	CCommandController();
	CCommandController(const CCommandController&) = delete;
	CCommandController& operator=(const CCommandController&) = delete;
	~CCommandController();
	int commandHandling(shared_ptr<CLink> shared_clientInfo, vector<string>& commandString, MessageStruct* sendClientMessage);
#pragma region get �Լ�
	//CRoomHandlere* getRoomHandlere() { return RoomHandlere; }
	CChannelHandler& getChannelHandler() { return ChannelHandler; }
	CChannelManager& getChannelManager() { return ChannelManager; }
	CRoomManager& getRoomManager() { return RoomManager; }
#pragma endregion
	bool deleteClientSocket(CLink& clientInfo); // �ι� ���� ȣ�� �Ǹ� ����
};