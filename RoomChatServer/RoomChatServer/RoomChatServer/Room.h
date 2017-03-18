#pragma once
#include<WinSock2.h>
#include<iostream>
#include<list>
#include"Channel.h"
using namespace std;
class CRoom
{
	char* RoomName;
	int ChannelNum;
	int RoomNum;
	// 현재 들어있는 방 인원
	int AmountPeople;
	SocketList* RoomSocketList;
	CRITICAL_SECTION* CS_RoomSocketList;
public:
	CRoom(int roomNo,int channelNum, char* roomName);
	~CRoom();
#pragma region push, erase 함수
	void pushSocket(SOCKET* clientSocket)
	{
		EnterCriticalSection(CS_RoomSocketList);
		RoomSocketList->push_front(clientSocket);
		increasePeople();
		LeaveCriticalSection(CS_RoomSocketList);
	}
	SocketListIt eraseSocket(SocketListIt socketListIt)
	{
		SocketListIt temp;
		EnterCriticalSection(CS_RoomSocketList);
		temp = RoomSocketList->erase(socketListIt);
		decreasePeople();
		LeaveCriticalSection(CS_RoomSocketList);
		return temp;
	}
#pragma endregion
#pragma region get,set 함수
	int getRoomNum() { return RoomNum; }
	int getChannelNum() { return ChannelNum; }
	char* getRoomName() { return RoomName; }
	SocketListIt getIterSocketBegin() { return RoomSocketList->begin(); }
	SocketListIt getIterSocketEnd() { return RoomSocketList->end(); }
	int getAmountPeople() { return AmountPeople; }
#pragma endregion
	void increasePeople() { AmountPeople++; }
	void decreasePeople() { if (AmountPeople > 0) AmountPeople--; }
};

