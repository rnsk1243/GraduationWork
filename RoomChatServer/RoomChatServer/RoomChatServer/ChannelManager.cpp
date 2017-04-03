#include "ChannelManager.h"



CChannelManager::CChannelManager():
	Channels(new ChannelList), 
	CS_Channel(new CRITICAL_SECTION())
{
	InitializeCriticalSection(CS_Channel);
}


CChannelManager::~CChannelManager()
{
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
	cout << ChannelNum << "�� ä���� �����ϴ�." << endl;
	return nullptr;
}
