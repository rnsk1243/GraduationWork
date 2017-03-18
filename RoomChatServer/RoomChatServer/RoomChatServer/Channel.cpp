#include "Channel.h"



CChannel::CChannel(int channelNum):ChannelNum(channelNum)
{
	ChannelSocketList = new SocketList();
	CS_ChannelSocketList = new CRITICAL_SECTION();
	InitializeCriticalSection(CS_ChannelSocketList);
}


CChannel::~CChannel()
{
	cout << "ä�� ����" << endl;
	delete &ChannelNum;
	delete ChannelSocketList;
	DeleteCriticalSection(CS_ChannelSocketList);
}
