#pragma once
#include"ChannelHandler.h"
#include"RoomHandler.h"
#include"ChannelManager.h"
#include"RoomManager.h"
#include"CardManager.h"

typedef vector<SOCKET> SocketVec;
typedef SocketVec::iterator SocketVecIt;

class CCommandController
{
	CChannelManager mChannelManager;
	CRoomManager mRoomManager;
	CChannelHandler mChannelHandler;
	CRoomHandler mRoomHandler;
	CCardManager mCardManager;
	bool CardSelect(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage);
	bool ReadyCommand(shared_ptr<CLink> shared_clientInfo, CLink*& clientInfo, int& channelNum);
	bool EnterRoom(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage);
	bool ChangeChannel(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage);
	bool MakeRoom(const string& roomName, shared_ptr<CLink> shared_clientInfo,const int& battingMoney, string& sendClientMessage);
	bool OutRoom(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage);
	bool MergeRoom(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage);
	bool CardCompose(shared_ptr<CLink> shared_clientInfo,const string& targetCardNum , const string& sourceCardNum, string& sendClientMessage);
	bool CardEvolution(shared_ptr<CLink> shared_clientInfo, const string& targetCardNum, string& sendClientMessage);
	bool SendAllReadyGameNotice(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage, SocketVec& clientSocks);
	bool IsHaveCard(shared_ptr<CLink> shared_clientInfo, int cardNum, string& sendClientMessage);
	bool SendBattingResult(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage, SocketVec& clientSocks);
	// ��� ó�� �Լ�(�� ����� ��)
public:
	CCommandController();
	CCommandController(const CCommandController&) = delete;
	CCommandController& operator=(const CCommandController&) = delete;
	~CCommandController();
	bool CommandHandling(shared_ptr<CLink> shared_clientInfo, vector<string>& commandString, string& sendClientMessage, SocketVec& clientSocks);
#pragma region get �Լ�
	//CRoomHandlere* getRoomHandlere() { return RoomHandlere; }
	CChannelHandler& GetChannelHandler() { return mChannelHandler; }
	CChannelManager& GetChannelManager() { return mChannelManager; }
	CRoomManager& GetRoomManager() { return mRoomManager; }
#pragma endregion
	bool DeleteClientSocket(CLink& clientInfo); // �ι� ���� ȣ�� �Ǹ� ����
};