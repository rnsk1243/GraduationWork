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
	ChannelList* Channels;
	CRITICAL_SECTION* CS_Channel;
	CChannelManager(const CChannelManager&);
	CChannelManager& operator=(const CChannelManager&);
public:
	CChannelManager();
	~CChannelManager();
	ChannelListIt getIterChannelBegin() { return Channels->begin(); }
	ChannelListIt getIterChannelEnd() { return Channels->end(); }
	void pushChannel(CChannel* newChannel)
	{
		EnterCriticalSection(CS_Channel);
		Channels->push_back(newChannel);
		LeaveCriticalSection(CS_Channel);
	}
	CChannel * getMyChannel(int ChannelNum);
};

