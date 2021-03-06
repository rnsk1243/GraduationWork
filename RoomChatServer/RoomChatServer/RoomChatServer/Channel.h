#pragma once
#include<WinSock2.h>
#include<list>
#include<iostream>
#include"Link.h"
using namespace std;
class CLink;
typedef list<CLink*> LinkList;
typedef LinkList::iterator LinkListIt;

class CChannel
{
	int ChannelNum;
	LinkList ClientInfos;
	CRITICAL_SECTION CS_MyInfoList;
	CChannel(const CChannel&);
	CChannel& operator=(const CChannel&);
public:
	CChannel(int channelNum);
	~CChannel();
#pragma region get,set �Լ�
	int getChannelNum() { return ChannelNum; }
	LinkListIt getIterMyInfoBegin() { return ClientInfos.begin(); }
	LinkListIt getIterMyInfoEnd() { return ClientInfos.end(); }
#pragma endregion

#pragma region push,erase �Լ�
	void pushClient(CLink* client)
	{
		EnterCriticalSection(&CS_MyInfoList);
		ClientInfos.push_back(client);
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

