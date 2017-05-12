#pragma once
#include"Link.h"
#include"CommandController.h"
//#include"graduationWork.pb.h"

class CActionNetWork
{
	CActionNetWork(const CActionNetWork&);
	CActionNetWork& operator=(const CActionNetWork&);
	// Size 보내기
	int sendSize(SOCKET& sock, int size, g_DataType type, int flags = 0);
	int recvSize(SOCKET& sock, g_DataSize& g_size, int flags = 0);
public:
	CActionNetWork();
	~CActionNetWork();
	// room에서 메시지 보내기
	int sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	int sendn(SOCKET& socket, g_Message& g_MS, int flags = 0); // 자신에게만 보내기
	int recvn(CLink& clientInfo, CCommandController& commandController, int flags = 0);
	int recvn(SOCKET& socket, g_Message& g_MS, int flags = 0); // 자신에게만 답장 받기
	int sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
	// transform 받기
	int recvnTransform(SOCKET& clientSocket, CLink& clientInfo, int recvSize, int flags = 0);
	// message 받기
	int recvnMessage(SOCKET& clientSocket, CLink& clientInfo, int recvSize, int flags = 0);
};

