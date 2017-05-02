#pragma once
#include<list>
#include<iostream>
#include"Channel.h"
using namespace std;
class CChannel;
class CLink;
typedef list<CChannel*> ChannelList;
typedef ChannelList::iterator ChannelListIt;

class CChannelManager
{
	ChannelList Channels;
	MUTEX RAII_ChannelManagerMUTEX;
	//CRITICAL_SECTION CS_Channel;
	CChannelManager& operator=(const CChannelManager&);
	CChannelManager(const CChannelManager&);
public:
	CChannelManager();
	~CChannelManager();
	ChannelListIt getIterChannelBegin() { return Channels.begin(); }
	ChannelListIt getIterChannelEnd() { return Channels.end(); }
	void pushChannel(CChannel* newChannel)
	{
		{
			ScopeLock<MUTEX> MU(RAII_ChannelManagerMUTEX); // lock
			Channels.push_back(newChannel);
		}// unlock
	}
	CChannel * getMyChannel(int ChannelNum);
};

