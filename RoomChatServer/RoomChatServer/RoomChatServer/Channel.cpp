#include "Channel.h"



CChannel::CChannel(int channelNum):
	ChannelNum(channelNum),
	ClientInfos(new LinkList()),
	CS_MyInfoList(new CRITICAL_SECTION)
{
	InitializeCriticalSection(CS_MyInfoList);
}


CChannel::~CChannel()
{
	cout << "채널 삭제" << endl;
	delete ClientInfos;
	DeleteCriticalSection(CS_MyInfoList);
}
