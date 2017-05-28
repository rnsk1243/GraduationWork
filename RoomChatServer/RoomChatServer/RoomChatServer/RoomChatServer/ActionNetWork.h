#pragma once
#include"Link.h"
#include"CommandController.h"



class CActionNetWork
{
	MessageStruct sendClientMessage; // 명령 처리 결과를 클라이언트에게 보내기 위한 메세지를 담을 공간
public:
	CActionNetWork();
	CActionNetWork(const CActionNetWork&) = delete;
	CActionNetWork& operator=(const CActionNetWork&) = delete;
	~CActionNetWork();
	// room에서 메시지 보내기
	int sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	// 내가 담당하는 클라이언트에게만 보내기
	int sendn(SOCKET& socket, MessageStruct& MS, int flags = 0);
	int recvn(shared_ptr<CLink> clientInfo, CCommandController& commandController, int flags = 0);
	// 내가 담당하는 클라이언트에게 답장 받기
	int recvn(SOCKET& socket, MessageStruct& MS, int flags = 0);
	int sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
	// 내가 담당하는 클라이언트에게 묻기
	int askClient(SOCKET& clientSocket, MessageStruct& MS, char* question);
	// 내가 담당하는 클라이언트에게 통보하기
	int notificationClient(SOCKET& clientSocket, MessageStruct& MS, char* notification);
};

