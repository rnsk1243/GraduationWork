#include "Channel.h"



CChannel::CChannel(int channelNum):
	ChannelNum(channelNum)
{
	InitializeCriticalSection(&CS_MyInfoList);
}


CChannel::~CChannel()
{
	cout << "ä�� ����" << endl;
	DeleteCriticalSection(&CS_MyInfoList);
}
