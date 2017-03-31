#pragma once
#include<WinSock2.h>
#include<list>
#include<iostream>
#include"Link.h"
using namespace std;
class CLink;
typedef list<CLink> LinkList;
typedef LinkList::iterator LinkListIt;

class CChannel
{
	int ChannelNum;
	LinkList ClientInfos;
	CRITICAL_SECTION CS_MyInfoList;
public:
	CChannel(int channelNum);
	CChannel(const CChannel& channel):
		ChannelNum(channel.ChannelNum),
		ClientInfos(channel.ClientInfos),
		CS_MyInfoList(channel.CS_MyInfoList)
	{
		//InitializeCriticalSection(&CS_MyInfoList);
	}
	~CChannel();
#pragma region get,set 함수
	int getChannelNum() { return ChannelNum; }
	LinkListIt getIterMyInfoBegin() { return ClientInfos.begin(); }
	LinkListIt getIterMyInfoEnd() { return ClientInfos.end(); }
#pragma endregion

#pragma region push,erase 함수
	void pushClient(CLink& client)
	{
		EnterCriticalSection(&CS_MyInfoList);
		ClientInfos.push_front(client);
		LeaveCriticalSection(&CS_MyInfoList);
	}
	LinkListIt eraseClient(LinkListIt myInfoListIt)
	{
		LinkListIt temp;
		EnterCriticalSection(&CS_MyInfoList);
		temp = ClientInfos.erase(myInfoListIt);
		LeaveCriticalSection(&CS_MyInfoList);
		return temp;
	}
#pragma endregion
};

