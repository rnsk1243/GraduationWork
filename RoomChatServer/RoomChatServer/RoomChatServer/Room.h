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
	LinkList ClientInfos;
	CRITICAL_SECTION CS_MyInfoList;
	void increasePeople() { AmountPeople++; }
	void decreasePeople() { if (AmountPeople > 0) AmountPeople--; }
	CRoom(const CRoom&) = delete;
	CRoom& operator=(const CRoom&) = delete;
	g_ReadySet mReadySet;
	g_CreateCharaterInfo* newCharaterInfo(CLink* client);
	void setReadyPlayerInfo(CLink* client);
public:
	CRoom(int roomNum,int channelNum, char* roomName);
	~CRoom();
#pragma region push, erase 함수
	void pushClient(CLink* client)
	{
		EnterCriticalSection(&CS_MyInfoList);
		ClientInfos.push_back(client);
		increasePeople();
		setReadyPlayerInfo(client);
		cout << "방 입장 성공 // 현재 방 인원 수 = " << AmountPeople << endl;
		LeaveCriticalSection(&CS_MyInfoList);
	}
	LinkListIt eraseClient(LinkListIt myInfoListIt)
	{
		LinkListIt temp;
		EnterCriticalSection(&CS_MyInfoList);
		temp = ClientInfos.erase(myInfoListIt);
		decreasePeople();
		LeaveCriticalSection(&CS_MyInfoList);
		return temp;
	}
#pragma endregion
#pragma region get,set 함수
	int getRoomNum() { return RoomNum; }
	int getChannelNum() { return ChannelNum; }
	char* getRoomName() { return RoomName; }
	LinkListIt getIterMyInfoBegin() { return ClientInfos.begin(); }
	LinkListIt getIterMyInfoEnd() { return ClientInfos.end(); }
	int getAmountPeople() { return AmountPeople; }
	g_ReadySet* getReadySet() { return &mReadySet; }
	bool updatePlayerReadyInfo();
#pragma endregion
	bool mergeRoom(CRoom* targetRoom)
	{
		EnterCriticalSection(&CS_MyInfoList);
		EnterCriticalSection(&targetRoom->CS_MyInfoList);
		// 실제 옮기기 전에 준비작업으로 room정보 수정
#pragma region 옮기는 방안에 있는 클라이언트들의 room정보 수정(방 번호라든지..)
		LinkListIt linkBegin = targetRoom->getIterMyInfoBegin();
		LinkListIt linkEnd = targetRoom->getIterMyInfoEnd();
		for (; linkBegin != linkEnd; ++linkBegin)
		{
			CLink* targetClient = (*linkBegin);
			targetClient->setMyRoomNum(RoomNum);
			increasePeople(); // 방 인원수 갱신
		}
#pragma endregion 
		ClientInfos.merge(targetRoom->ClientInfos); // 실제 옮김

		LeaveCriticalSection(&targetRoom->CS_MyInfoList);
		LeaveCriticalSection(&CS_MyInfoList);
		return true;
	}
	bool isTeamBalance();
};

