#pragma once
#include<WinSock2.h>
#include<iostream>
#include<list>
#include"Channel.h"
class CChannel;
class CLink;
using namespace std;


class CRoom
{
	char* RoomName;
	int ChannelNum;
	int RoomNum;
	// 현재 들어있는 방 인원
	int AmountPeople;
	LinkList* ClientInfos;
	CRITICAL_SECTION* CS_MyInfoList;
	void increasePeople() { AmountPeople++; }
	void decreasePeople() { if (AmountPeople > 0) AmountPeople--; }
public:
	CRoom(int roomNum,int channelNum, char* roomName);
	~CRoom();
#pragma region push, erase 함수
	void pushClient(CLink* client)
	{
		EnterCriticalSection(CS_MyInfoList);
		ClientInfos->push_front(client);
		increasePeople();
		LeaveCriticalSection(CS_MyInfoList);
	}
	LinkListIt eraseClient(LinkListIt myInfoListIt)
	{
		LinkListIt temp;
		EnterCriticalSection(CS_MyInfoList);
		temp = ClientInfos->erase(myInfoListIt);
		decreasePeople();
		LeaveCriticalSection(CS_MyInfoList);
		return temp;
	}
#pragma endregion
#pragma region get,set 함수
	int getRoomNum() { return RoomNum; }
	int getChannelNum() { return ChannelNum; }
	char* getRoomName() { return RoomName; }
	LinkListIt getIterMyInfoBegin() { return ClientInfos->begin(); }
	LinkListIt getIterMyInfoEnd() { return ClientInfos->end(); }
	int getAmountPeople() { return AmountPeople; }
#pragma endregion
};

