#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ErrorHandler.h"
#include"Channel.h"
#include"Link.h"
#include"ChannelManager.h"

class CChannelHandler
{
public:
	CChannelHandler();
	~CChannelHandler();
	CChannelHandler(const CChannelHandler&) = delete;
	CChannelHandler& operator=(const CChannelHandler&) = delete;
	// 채널옮김 함수(옮기는 소켓, 옮기고싶은 채널 번호)
	bool MoveNextChannel(const shared_ptr<CLink>& shared_clientInfo, CChannelManager& channelManager, int targetChannelNo);
	// 채널 에서 나가기 (나가고자하는 소켓)
	bool ExitChannel(CLink& clientInfo, CChannelManager& channelManager);

};

