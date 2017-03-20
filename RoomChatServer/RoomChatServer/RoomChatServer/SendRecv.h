#pragma once
#include"Link.h"
#include"CommandController.h"

class CSendRecv
{
public:
	CSendRecv();
	~CSendRecv();
	// room에서 메시지 보내기
	int sendn(CLink* clientInfo, CRoomChannelManager* roomChannelManager, int flags = 0);
	int recvn(CLink* clientInfo, CCommandController* commandController, int flags = 0);
};

