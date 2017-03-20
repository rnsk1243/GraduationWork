#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"Channel.h"
#include"Link.h"
#include"RoomChannelManager.h"


class CChannelHandler
{
public:
	CChannelHandler();
	~CChannelHandler();
	// ä�οű� �Լ�(�ű�� ����, �ű����� ä�� ��ȣ)
	bool enterChannel(CLink* clientInfo, CRoomChannelManager* roomChannelManager, int targetChannelNo);
	// ä�� ���� ������ (���������ϴ� ����)
	bool exitChannel(CLink* clientInfo, CRoomChannelManager* roomChannelManager);
	bool pushChannel(CChannel* newChannel);
};

