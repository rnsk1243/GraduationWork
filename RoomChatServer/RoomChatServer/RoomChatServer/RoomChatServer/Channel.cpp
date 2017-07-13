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
