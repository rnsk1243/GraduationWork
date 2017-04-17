#include "ChannelManager.h"
#include"CommandController.h"


CChannelManager::CChannelManager(int channelAmount):
	Channels(new ChannelList), 
	CS_Channel(new CRITICAL_SECTION())
{
	InitializeCriticalSection(CS_Channel);
	for (int i = EnterChannelNum; i <= channelAmount; i++)
	{
		CChannel* newChannel = new CChannel(i);
		pushChannel(newChannel);
	}
}


CChannelManager::~CChannelManager()
{
	ChannelListIt begin = getIterChannelBegin();
	ChannelListIt end = getIterChannelEnd();
	for (; begin != end; ++begin)
	{
		delete(*begin);
	}
	Channels->clear();
	DeleteCriticalSection(CS_Channel);
}

CChannel * CChannelManager::getMyChannel(int ChannelNum)
{
	ChannelListIt iterBegin = Channels->begin();
	ChannelListIt iterEnd = Channels->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->getChannelNum() == ChannelNum)
			return *iterBegin;
	}
	cout << ChannelNum << "번 채널이 없습니다." << endl;
	return nullptr;
}
