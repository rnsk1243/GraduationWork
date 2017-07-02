#include "ChannelManager.h"
#include"CommandController.h"


CChannelManager::CChannelManager()
{
	//InitializeCriticalSection(&CS_Channel);
	for (int i = EnterChannelNum; i <= ChannelAmount; i++)
	{
		shared_ptr<CChannel> newChannel(new CChannel(i));
		//CChannel* newChannel = new CChannel(i);
		PushChannel(newChannel);
	}
}


CChannelManager::~CChannelManager()
{
	cout << "ChannelManager �Ҹ��� ȣ��" << endl;
	//ChannelListIt begin = getIterChannelBegin();
	//ChannelListIt end = getIterChannelEnd();
	//for (; begin != end; ++begin)
	//{
	//	delete(*begin);
	//}
	//Channels.clear();
	//DeleteCriticalSection(&CS_Channel);
}

CChannel * CChannelManager::GetMyChannel(int ChannelNum)
{
	ChannelListIt iterBegin = mChannels.begin();
	ChannelListIt iterEnd = mChannels.end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->GetChannelNum() == ChannelNum)
			return (*iterBegin).get();
	}
	cout << ChannelNum << "�� ä���� �����ϴ�." << endl;
	return nullptr;
}
