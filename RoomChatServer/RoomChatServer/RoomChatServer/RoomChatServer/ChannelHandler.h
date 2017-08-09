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
	// ä�οű� �Լ�(�ű�� ����, �ű����� ä�� ��ȣ)
	bool MoveNextChannel(const shared_ptr<CLink>& shared_clientInfo, CChannelManager& channelManager, int targetChannelNo);
	// ä�� ���� ������ (���������ϴ� ����)
	bool ExitChannel(CLink& clientInfo, CChannelManager& channelManager);

};

