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
	LinkList ClientInfos;
	char* RoomName;
	int RoomNum;
	// 현재 들어있는 방 인원
	int AmountPeople;
	int ChannelNum;
	CRITICAL_SECTION CS_MyInfoList;
	void increasePeople() { AmountPeople++; }
	void decreasePeople() { if (AmountPeople > 0) AmountPeople--; }
public:
	CRoom(int roomNum,int channelNum, char* roomName);
	// 복사생성자
	CRoom(const CRoom& room):
		ClientInfos(room.ClientInfos),
		RoomNum(room.RoomNum),
		AmountPeople(room.AmountPeople),
		ChannelNum(room.ChannelNum),
		CS_MyInfoList(room.CS_MyInfoList)
	{
		if (room.RoomName)
		{
			int length = strlen(room.RoomName) + 1;
			RoomName = new char[length];
			for (int i = 0; i < length - 1; i++)
			{
				RoomName[i] = room.RoomName[i];
			}
			RoomName[length - 1] = '\0';
		}
		//InitializeCriticalSection(&CS_MyInfoList); 복사생성자에서도 해줘야하나요?
	}
	~CRoom();
#pragma region push, erase 함수
	void pushClient(CLink& client)
	{
		EnterCriticalSection(&CS_MyInfoList);
		ClientInfos.push_front(client);
		increasePeople();
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
#pragma endregion
	bool mergeRoom(CRoom& targetRoom)
	{
		EnterCriticalSection(&CS_MyInfoList);
		EnterCriticalSection(&targetRoom.CS_MyInfoList);
		// 실제 옮기기 전에 준비작업으로 room정보 수정
#pragma region 옮기는 방안에 있는 클라이언트들의 room정보 수정(방 번호라든지..)
		LinkListIt linkBegin = targetRoom.getIterMyInfoBegin();
		LinkListIt linkEnd = targetRoom.getIterMyInfoEnd();
		for (; linkBegin != linkEnd; ++linkBegin)
		{
			CLink targetClient = *linkBegin; // 복사생성자 호출
			targetClient.setMyRoomNum(RoomNum);
			increasePeople(); // 방 인원수 갱신
		}
#pragma endregion 
		ClientInfos.merge(targetRoom.ClientInfos); // 실제 옮김
		LeaveCriticalSection(&targetRoom.CS_MyInfoList);
		LeaveCriticalSection(&CS_MyInfoList);
		return true;
	}
};

