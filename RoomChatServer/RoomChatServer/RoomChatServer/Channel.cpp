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
	LinkListIt begin = getIterMyInfoBegin();
	LinkListIt end = getIterMyInfoEnd();
	for (; begin != end; ++begin)
	{
		delete(*begin);
	}
	ClientInfos->clear();
	DeleteCriticalSection(CS_MyInfoList);
}
