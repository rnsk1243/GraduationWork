#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"Channel.h"
#include"Link.h"
#include"ChannelManager.h"


class CChannelHandler
{
	CChannelHandler(const CChannelHandler&);
	CChannelHandler& operator=(const CChannelHandler&);
public:
	CChannelHandler();
	~CChannelHandler();

	// 채널옮김 함수(옮기는 소켓, 옮기고싶은 채널 번호)
	bool enterChannel(CLink* clientInfo, CChannelManager& channelManager, int targetChannelNo);
	// 채널 에서 나가기 (나가고자하는 소켓)
	bool exitChannel(CLink& clientInfo, CChannelManager& channelManager);

};

