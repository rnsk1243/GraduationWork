#include "Channel.h"
#include"ErrorHandler.h"

CChannel::CChannel(int channelNum):
	mChannelNum(channelNum),
	mPeopleAmount(0)
{
	//InitializeCriticalSection(&CS_MyInfoList);
}


CChannel::~CChannel()
{
	cout << "채널 삭제" << endl;
}

int CChannel::GetChannelNum()
{
	return mChannelNum;
}

bool CChannel::PushClient(const shared_ptr<CLink>& shared_client, const int& channelNumber)
{
	if (nullptr == shared_client.get())
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
		return false;
	}
	ScopeLock<MUTEX> MU(mRAII_ChannelMUTEX);
	mClientInfos.push_back(shared_client);
	shared_client.get()->SetMyChannelNum(channelNumber);
	mPeopleAmount++;
	return true;
}

LinkListIt CChannel::EraseClient(const shared_ptr<CLink>& shared_clientInfo)
{
	LinkListIt eraseClientIter = find(mClientInfos.begin(), mClientInfos.end(), shared_clientInfo);
	{
		ScopeLock<MUTEX> MU(mRAII_ChannelMUTEX);
		eraseClientIter = mClientInfos.erase(eraseClientIter);
		if (0 < mPeopleAmount)
		{
			--mPeopleAmount;
		}
	}
	return eraseClientIter;
}

void CChannel::Broadcast(const string & message, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		(*clientIterBegin).get()->SendnMine(message, flags);
	}
}

void CChannel::Talk(const LinkPtr & myClient, const string & message, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	LinkListIt myIter = find(mClientInfos.begin(), mClientInfos.end(), myClient);
	if (mClientInfos.end() == myIter)
	{
		Broadcast(message, flags);
		return;
	}
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		if (clientIterBegin != myIter)
		{
			(*clientIterBegin).get()->SendnMine(message, flags);
		}
	}
}
