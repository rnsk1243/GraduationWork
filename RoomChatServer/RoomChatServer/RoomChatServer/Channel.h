#pragma once
#include<WinSock2.h>
#include<list>
#include<iostream>
#include"Link.h"
#include"RAII.h"
using namespace std;
class CLink;
typedef list<CLink*> LinkList;
typedef LinkList::iterator LinkListIt;

class CChannel
{
	int ChannelNum;
	LinkList ClientInfos;
	//CRITICAL_SECTION CS_MyInfoList;
	CChannel(const CChannel&);
	CChannel& operator=(const CChannel&);
	MUTEX RAII_ChannelMUTEX;
	//CRITICALSECTION CS;
public:
	CChannel(int channelNum);
	~CChannel();
#pragma region get,set 함수
	int getChannelNum() { return ChannelNum; }
	LinkListIt getIterMyInfoBegin() { return ClientInfos.begin(); }
	LinkListIt getIterMyInfoEnd() { return ClientInfos.end(); }
#pragma endregion

#pragma region push,erase 함수
	void pushClient(CLink* client)
	{
		{
			ScopeLock<MUTEX> MU(RAII_ChannelMUTEX);
			ClientInfos.push_back(client);
		}
	}
	LinkListIt eraseClient(LinkListIt myInfoListIt)
	{
		LinkListIt temp;
		{
			ScopeLock<MUTEX> MU(RAII_ChannelMUTEX);
			temp = ClientInfos.erase(myInfoListIt);
		}
		return temp;
	}
#pragma endregion
};

