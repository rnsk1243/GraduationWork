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
	/*LinkListIt begin = getIterMyInfoBegin();
	LinkListIt end = getIterMyInfoEnd();
	for (; begin != end; ++begin)
	{
		delete(*begin);
	}
	ClientInfos.clear();*/
	// DeleteCriticalSection(&CS);
}

bool CChannel::GetChannelSockets(vector<SOCKET>& channelSockets, bool isMyInclude, const SOCKET* myClientSock)
{
	LinkListIt linkBegin = GetIterMyInfoBegin();

	for (; linkBegin != GetIterMyInfoEnd(); ++linkBegin)
	{
		if ((*linkBegin).use_count() > 0)
		{
			channelSockets.reserve(mPeopleAmount);
			if (true == isMyInclude)
			{
				channelSockets.push_back((*linkBegin).get()->GetClientSocket());
			}
			else
			{
				if (nullptr == myClientSock)
				{
					ErrorHandStatic->ErrorHandler(ERROR_GET_CHANNEL_SOCKET_MYCLIENT_NULLPTR);
					return false;
				}
				for (; linkBegin != GetIterMyInfoEnd(); ++linkBegin)
				{
					if (*myClientSock != (*linkBegin).get()->GetClientSocket())
					{
						channelSockets.push_back((*linkBegin).get()->GetClientSocket());
					}
				}
			}
			return true;
		}
		else
		{
			ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
			return false;
		}
	}
	return false;
}

void CChannel::PushClient(const shared_ptr<CLink>& shared_client)
{
	if (nullptr == shared_client.get())
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
		return;
	}
	ScopeLock<MUTEX> MU(mRAII_ChannelMUTEX);
	mClientInfos.push_back(shared_client);
	mPeopleAmount++;
}
