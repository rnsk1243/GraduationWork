#include "Channel.h"



CChannel::CChannel(int channelNum):
	mChannelNum(channelNum)
{
	//InitializeCriticalSection(&CS_MyInfoList);
}


CChannel::~CChannel()
{
	cout << "ä�� ����" << endl;
	/*LinkListIt begin = getIterMyInfoBegin();
	LinkListIt end = getIterMyInfoEnd();
	for (; begin != end; ++begin)
	{
		delete(*begin);
	}
	ClientInfos.clear();*/
	// DeleteCriticalSection(&CS);
}
