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
	void pushChannel(shared_ptr<CChannel> shared_newChannel)
	{
		if (0 >= shared_newChannel.use_count())
		{
			CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
			return;
		}
		//{
		// ä���� ���� �ǰų� ���� �����Ƿ� lock ���ʿ�� ����.
			//ScopeLock<MUTEX> MU(RAII_ChannelManagerMUTEX); // lock
			//Channels.push_back(newChannel);
		//}// unlock
		Channels.push_back(shared_newChannel);
	}
public:
	CChannelManager();
	~CChannelManager();
	CChannelManager& operator=(const CChannelManager&) = delete;
	CChannelManager(const CChannelManager&) = delete;
	ChannelListIt getIterChannelBegin() { return Channels.begin(); }
	ChannelListIt getIterChannelEnd() { return Channels.end(); }
	CChannel * getMyChannel(int ChannelNum);
};

