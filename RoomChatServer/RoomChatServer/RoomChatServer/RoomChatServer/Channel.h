#pragma once
#include<WinSock2.h>
#include<list>
#include<iostream>
#include"Link.h"
#include"RAII.h"
using namespace std;
class CLink;

typedef list<LinkPtr> LinkList;
typedef LinkList::iterator LinkListIt;

class CChannel
{
	int mPeopleAmount;
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
	int GetChannelNum();
	// 채널에 속해 있는 소켓 가져오기.(결과 담을 벡터)
	bool PushClient(const LinkPtr& shared_client, const int& channelNumber);
	LinkListIt EraseClient(const LinkPtr& shared_clientInfo);
	void Broadcast(const string& message, int flags = 0);
	void Talk(const LinkPtr& myClient, const string& message, int flags = 0);
};

