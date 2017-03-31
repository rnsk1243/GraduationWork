#include "Channel.h"



CChannel::CChannel(int channelNum):
	ChannelNum(channelNum)
{
	InitializeCriticalSection(&CS_MyInfoList);
}


CChannel::~CChannel()
{
	cout << "채널 삭제" << endl;
	DeleteCriticalSection(&CS_MyInfoList);
}
