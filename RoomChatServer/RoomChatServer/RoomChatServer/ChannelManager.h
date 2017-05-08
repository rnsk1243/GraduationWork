#pragma once
#include<list>
#include<iostream>
#include"Channel.h"
using namespace std;
class CChannel;
class CLink;
//typedef list<CChannel*> ChannelList;
typedef list<shared_ptr<CChannel>> ChannelList;
typedef ChannelList::iterator ChannelListIt;

class CChannelManager
{
	ChannelList Channels;
	MUTEX RAII_ChannelManagerMUTEX;
	//CRITICAL_SECTION CS_Channel;
public:
	CChannelManager();
	~CChannelManager();
	CChannelManager& operator=(const CChannelManager&) = delete;
	CChannelManager(const CChannelManager&) = delete;
	ChannelListIt getIterChannelBegin() { return Channels.begin(); }
	ChannelListIt getIterChannelEnd() { return Channels.end(); }
	void pushChannel(shared_ptr<CChannel> newChannel)
	{
		//{
			//ScopeLock<MUTEX> MU(RAII_ChannelManagerMUTEX); // lock
			//Channels.push_back(newChannel);
		//}// unlock
		Channels.push_back(newChannel);
	}
	CChannel * getMyChannel(int ChannelNum);
};

