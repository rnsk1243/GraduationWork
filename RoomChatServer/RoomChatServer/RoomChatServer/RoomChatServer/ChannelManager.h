#pragma once
#include<vector>
#include<iostream>
#include"Channel.h"

using namespace std;
class CChannel;
class CLink;
class CErrorHandler;
//typedef list<CChannel*> ChannelList;
typedef vector<shared_ptr<CChannel>> ChannelVector;
typedef ChannelVector::iterator ChannelVecIt;

class CChannelManager
{
	ChannelVector mChannels;
	MUTEX mRAII_ChannelManagerMUTEX;
	//CRITICAL_SECTION CS_Channel;
	void PushChannel(const shared_ptr<CChannel>& shared_newChannel);
public:
	CChannelManager();
	~CChannelManager();
	CChannelManager& operator=(const CChannelManager&) = delete;
	CChannelManager(const CChannelManager&) = delete;
	ChannelVecIt GetIterChannelBegin() { return mChannels.begin(); }
	ChannelVecIt GetIterChannelEnd() { return mChannels.end(); }
	CChannel * GetMyChannel(int ChannelNum);
};

