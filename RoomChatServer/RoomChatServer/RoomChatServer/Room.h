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
	// ���� ����ִ� �� �ο�
	int AmountPeople;
	int ChannelNum;
	CRITICAL_SECTION CS_MyInfoList;
	void increasePeople() { AmountPeople++; }
	void decreasePeople() { if (AmountPeople > 0) AmountPeople--; }
public:
	CRoom(int roomNum,int channelNum, char* roomName);
	// ���������
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
		//InitializeCriticalSection(&CS_MyInfoList); ��������ڿ����� ������ϳ���?
	}
	~CRoom();
#pragma region push, erase �Լ�
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
#pragma region get,set �Լ�
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
		// ���� �ű�� ���� �غ��۾����� room���� ����
#pragma region �ű�� ��ȿ� �ִ� Ŭ���̾�Ʈ���� room���� ����(�� ��ȣ�����..)
		LinkListIt linkBegin = targetRoom.getIterMyInfoBegin();
		LinkListIt linkEnd = targetRoom.getIterMyInfoEnd();
		for (; linkBegin != linkEnd; ++linkBegin)
		{
			CLink targetClient = *linkBegin; // ��������� ȣ��
			targetClient.setMyRoomNum(RoomNum);
			increasePeople(); // �� �ο��� ����
		}
#pragma endregion 
		ClientInfos.merge(targetRoom.ClientInfos); // ���� �ű�
		LeaveCriticalSection(&targetRoom.CS_MyInfoList);
		LeaveCriticalSection(&CS_MyInfoList);
		return true;
	}
};

