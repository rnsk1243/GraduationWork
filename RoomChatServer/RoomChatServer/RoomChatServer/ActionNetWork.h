#pragma once
#include"Link.h"
#include"CommandController.h"

const int NullNameError = -1;

class CActionNetWork
{
public:
	CActionNetWork();
	~CActionNetWork();
	// room���� �޽��� ������
	int sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	int recvn(CLink& clientInfo, CCommandController& commandController, int flags = 0);
	int sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
};

