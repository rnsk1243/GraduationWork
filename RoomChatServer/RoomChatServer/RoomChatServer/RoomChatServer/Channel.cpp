#include "Channel.h"
#include"ErrorHandler.h"

CChannel::CChannel(int channelNum):
	mChannelNum(channelNum)
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
				if (*myClientSock != (*linkBegin).get()->GetClientSocket())
				{
					channelSockets.push_back((*linkBegin).get()->GetClientSocket());
				}
			}
		}
		else
		{
			ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
			return false;
		}
	}
	return true;
}

void CChannel::PushClient(shared_ptr<CLink> shared_client)
{
	if (0 >= shared_client.use_count())
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
		return;
	}
	ScopeLock<MUTEX> MU(mRAII_ChannelMUTEX);
	mClientInfos.push_back(shared_client);
}
