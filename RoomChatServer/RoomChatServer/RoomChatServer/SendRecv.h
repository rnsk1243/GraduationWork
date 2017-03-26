#pragma once
#include"Link.h"
#include"CommandController.h"

const int NullNameError = -1;

class CSendRecv
{
public:
	CSendRecv();
	~CSendRecv();
	// room에서 메시지 보내기
	int sendn(CLink* clientInfo, CRoomChannelManager* roomChannelManager, int flags = 0);
	int recvn(CLink* clientInfo, CCommandController* commandController, int flags = 0);
	int sendMyName(SOCKET* clientSocket, CLink* clientInfo, CRoomChannelManager* roomChannelManager, int flags = 0);
};

