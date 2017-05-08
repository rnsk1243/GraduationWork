#include "ChannelManager.h"
#include"CommandController.h"


CChannelManager::CChannelManager()
{
	//InitializeCriticalSection(&CS_Channel);
	for (int i = EnterChannelNum; i <= ChannelAmount; i++)
	{
		shared_ptr<CChannel> newChannel(new CChannel(i));
		//CChannel* newChannel = new CChannel(i);
		pushChannel(newChannel);
	}
}


CChannelManager::~CChannelManager()
{
	cout << "ChannelManager 소멸자 호출" << endl;
	//ChannelListIt begin = getIterChannelBegin();
	//ChannelListIt end = getIterChannelEnd();
	//for (; begin != end; ++begin)
	//{
	//	delete(*begin);
	//}
	//Channels.clear();
	//DeleteCriticalSection(&CS_Channel);
}

CChannel * CChannelManager::getMyChannel(int ChannelNum)
{
	ChannelListIt iterBegin = Channels.begin();
	ChannelListIt iterEnd = Channels.end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->getChannelNum() == ChannelNum)
			return (*iterBegin).get();
	}
	cout << ChannelNum << "번 채널이 없습니다." << endl;
	return nullptr;
}
