#pragma once
#include<list>
#include<iostream>
#include"Channel.h"

using namespace std;
class CChannel;
class CLink;
class CErrorHandler;
//typedef list<CChannel*> ChannelList;
typedef list<shared_ptr<CChannel>> ChannelList;
typedef ChannelList::iterator ChannelListIt;

class CChannelManager
{
	ChannelList mChannels;
	MUTEX mRAII_ChannelManagerMUTEX;
	//CRITICAL_SECTION CS_Channel;
	void PushChannel(shared_ptr<CChannel> shared_newChannel);
public:
	CChannelManager();
	~CChannelManager();
	CChannelManager& operator=(const CChannelManager&) = delete;
	CChannelManager(const CChannelManager&) = delete;
	ChannelListIt GetIterChannelBegin() { return mChannels.begin(); }
	ChannelListIt GetIterChannelEnd() { return mChannels.end(); }
	CChannel * GetMyChannel(int ChannelNum);
};

