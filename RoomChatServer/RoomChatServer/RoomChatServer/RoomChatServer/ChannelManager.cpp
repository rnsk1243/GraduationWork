#include "ChannelManager.h"
#include"ErrorHandler.h"
#include"CommandController.h"

void CChannelManager::PushChannel(shared_ptr<CChannel> shared_newChannel)
{
	if (0 >= shared_newChannel.use_count())
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_CHANNEL_COUNT_ZORO);
		return;
	}
	//{
	// 채널이 삭제 되거나 하지 않으므로 lock 걸필요는 없다.
	//ScopeLock<MUTEX> MU(RAII_ChannelManagerMUTEX); // lock
	//Channels.push_back(newChannel);
	//}// unlock
	mChannels.push_back(shared_newChannel);
}

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

CChannel * CChannelManager::GetMyChannel(int ChannelNum)
{
	ChannelListIt iterBegin = mChannels.begin();
	ChannelListIt iterEnd = mChannels.end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->GetChannelNum() == ChannelNum)
			return (*iterBegin).get();
	}
	cout << ChannelNum << "번 채널이 없습니다." << endl;
	ErrorHandStatic->ErrorHandler(ERROR_GET_CHANNEL);
	return nullptr;
}
