#include "ChannelManager.h"
#include"ErrorHandler.h"
#include"CommandController.h"

void CChannelManager::PushChannel(const ChannelPtr& shared_newChannel)
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
	mChannels.reserve(ChannelAmount);
	for (int i = EnterChannelNum; i <= ChannelAmount; i++)
	{
		shared_ptr<CChannel> newChannel(new CChannel(i));
		PushChannel(newChannel);
	}
}

CChannel * CChannelManager::GetMyChannel(int ChannelNum)
{
	ChannelVecIt iterBegin = mChannels.begin();
	ChannelVecIt iterEnd = mChannels.end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->GetChannelNum() == ChannelNum)
			return (*iterBegin).get();
	}
	cout << ChannelNum << "번 채널이 없습니다." << endl;
	ErrorHandStatic->ErrorHandler(ERROR_GET_CHANNEL);
	return nullptr;
}

CChannelManager::~CChannelManager()
{
	cout << "ChannelManager 소멸자 호출" << endl;
}

bool CChannelManager::MoveChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber)
{
	if (nullptr == shared_clientInfo.get())
		return false;
	ChannelVecIt iterBegin = mChannels.begin();
	CChannel* nextChannel;
	if (ChannelAmount < moveChannelNumber || EnterChannelNum > moveChannelNumber)
	{
		nextChannel = GetMyChannel(shared_clientInfo.get()->GetMyChannelNum());
	}
	else
	{
		nextChannel = GetMyChannel(moveChannelNumber);
	}
	if (nullptr == nextChannel)
		return false;
	return nextChannel->PushClient(shared_clientInfo, moveChannelNumber);
}

bool CChannelManager::ExitChannel(const LinkPtr& shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return false;
	int curChannelNumber = client->GetMyChannelNum();
	CChannel* nextChannel = GetMyChannel(curChannelNumber);
	if (nullptr == nextChannel)
		return false;
	nextChannel->EraseClient(shared_clientInfo);
	return true;
}

void CChannelManager::Talk(const LinkPtr & shared_clientInfo, const string & message, int flags)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return;
	CChannel* myChannel = GetMyChannel(client->GetMyChannelNum());
	if (nullptr != myChannel)
	{
		myChannel->Talk(shared_clientInfo, message, flags);
	}
}

