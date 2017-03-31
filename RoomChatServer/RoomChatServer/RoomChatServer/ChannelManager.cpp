#include "ChannelManager.h"



CChannelManager::CChannelManager()
{
	InitializeCriticalSection(&CS_Channel);
}


CChannelManager::~CChannelManager()
{
}

CChannel CChannelManager::getMyChannel(int ChannelNum)
{
	ChannelListIt iterBegin = Channels.begin();
	ChannelListIt iterEnd = Channels.end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (iterBegin->getChannelNum() == ChannelNum)
			return *iterBegin;
	}
	cout << ChannelNum << "번 채널이 없습니다." << endl;
	return *iterBegin; // nullptr의 역참조하면 오류 날텐데?? 뭘 리턴해야할까..?
}
