#pragma once
#include<WinSock2.h>
#include<list>
#include<iostream>
#include"Link.h"
#include"RAII.h"
#include"ErrorHandler.h"
using namespace std;
class CLink;
//typedef list<CLink*> LinkList;
typedef list<shared_ptr<CLink>> LinkList;
typedef LinkList::iterator LinkListIt;

class CChannel
{
	int mChannelNum;
	LinkList mClientInfos;
	//CRITICAL_SECTION CS_MyInfoList;
	MUTEX mRAII_ChannelMUTEX;
	//CRITICALSECTION CS;
public:
	CChannel(int channelNum);
	~CChannel();
	CChannel(const CChannel&) = delete;
	CChannel& operator=(const CChannel&) = delete;
#pragma region get,set 함수
	int GetChannelNum() { return mChannelNum; }
	LinkListIt GetIterMyInfoBegin() { return mClientInfos.begin(); }
	LinkListIt GetIterMyInfoEnd() { return mClientInfos.end(); }
#pragma endregion

#pragma region push,erase 함수
	void PushClient(shared_ptr<CLink> shared_client)
	{
		if (0 >= shared_client.use_count())
		{
			CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
			return;
		}
		ScopeLock<MUTEX> MU(mRAII_ChannelMUTEX);
		mClientInfos.push_back(shared_client);
	}
	LinkListIt EraseClient(LinkListIt myInfoListIt)
	{
		LinkListIt temp;
		{
			ScopeLock<MUTEX> MU(mRAII_ChannelMUTEX);
			temp = mClientInfos.erase(myInfoListIt);
		}
		return temp;
	}
#pragma endregion
};

