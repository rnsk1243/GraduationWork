#pragma once
#include<WinSock2.h>
#include<list>
#include<iostream>
using namespace std;
typedef list<SOCKET*> SocketList;
typedef SocketList::iterator SocketListIt;

class CChannel
{
	int ChannelNum;
	SocketList* ChannelSocketList;
	CRITICAL_SECTION* CS_ChannelSocketList;
public:
	CChannel(int channelNum);
	~CChannel();
#pragma region get,set 함수
	int getChannelNum() { return ChannelNum; }
	SocketListIt getIterBegin() { return ChannelSocketList->begin(); }
	SocketListIt getIterEnd() { return ChannelSocketList->end(); }
#pragma endregion

#pragma region push,erase 함수
	void pushSocket(SOCKET* clientSocket)
	{
		EnterCriticalSection(CS_ChannelSocketList);
		ChannelSocketList->push_front(clientSocket);
		LeaveCriticalSection(CS_ChannelSocketList);
	}
	SocketListIt eraseSocket(SocketListIt socketListIt)
	{
		SocketListIt temp;
		EnterCriticalSection(CS_ChannelSocketList);
		temp = ChannelSocketList->erase(socketListIt);
		LeaveCriticalSection(CS_ChannelSocketList);
		return temp;
	}
#pragma endregion

};

