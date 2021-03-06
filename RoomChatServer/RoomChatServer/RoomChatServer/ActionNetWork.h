#pragma once
#include"Link.h"
#include"CommandController.h"
//#include"graduationWork.pb.h"

class CActionNetWork
{
	CActionNetWork(const CActionNetWork&);
	CActionNetWork& operator=(const CActionNetWork&);
	// Size 보내기
	int sendSize(SOCKET& sock, int size, DataType type, int flags = 0);
	int recvSize(SOCKET& sock, DataSize& g_size, int flags = 0);
public:
	CActionNetWork();
	~CActionNetWork();
	// room에서 메시지 보내기
	int sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	// 자신에게 보내기
	int sendn(SOCKET& socket, Message& g_MS, int flags = 0);
	int recvn(CLink& clientInfo, CCommandController& commandController, int flags = 0);
	// 자신에게 답장 받기
	int recvn(SOCKET& socket, Message& g_MS, int flags = 0);
	int sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
};

