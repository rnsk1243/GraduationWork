#pragma once
#include<WinSock2.h>
#include<iostream>
#include<list>
#include"Channel.h"
#include"RAII.h"
class CChannel;
class CLink;
using namespace std;


class CRoom
{
	char* RoomName;
	int ChannelNum;
	int RoomNum;
	// ���� ����ִ� �� �ο�
	int AmountPeople;
	LinkList ClientInfos;
	MUTEX RAII_RoomMUTEX;
	//CRITICALSECTION CT;
	void increasePeople() { AmountPeople++; }
	void decreasePeople() { if (AmountPeople > 0) AmountPeople--; }
public:
	CRoom(const CRoom&) = delete;
	CRoom& operator=(const CRoom&) = delete;
	CRoom(int roomNum,int channelNum, char* roomName);
	~CRoom();
#pragma region push, erase �Լ�
	void pushClient(shared_ptr<CLink> shared_client)
	{
		ScopeLock<MUTEX> MU(RAII_RoomMUTEX);
		ClientInfos.push_back(shared_client);
		increasePeople();
	}
	LinkListIt eraseClient(LinkListIt myInfoListIt)
	{
		LinkListIt temp;
		{
			ScopeLock<MUTEX> MU(RAII_RoomMUTEX);
			temp = ClientInfos.erase(myInfoListIt);
			decreasePeople();
		}
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
	bool mergeRoom(CRoom* targetRoom)
	{
		{
			ScopeLock<MUTEX> MU(RAII_RoomMUTEX); // rock0
			{
				ScopeLock<MUTEX> MU(targetRoom->RAII_RoomMUTEX); // rock1
				// ���� �ű�� ���� �غ��۾����� room���� ����
#pragma region �ű�� ��ȿ� �ִ� Ŭ���̾�Ʈ���� room���� ����(�� ��ȣ�����..)
				LinkListIt linkBegin = targetRoom->getIterMyInfoBegin();
				LinkListIt linkEnd = targetRoom->getIterMyInfoEnd();
				for (; linkBegin != linkEnd; ++linkBegin)
				{
					CLink* targetClient = (*linkBegin).get();
					targetClient->setMyRoomNum(RoomNum);
					increasePeople(); // �� �ο��� ����
				}
#pragma endregion 
//				ClientInfos.sort();
//				targetRoom->ClientInfos.sort();
				ClientInfos.merge(targetRoom->ClientInfos); // ���� �ű�
			} // rock1 unlock
		} // rock0 unlock
		return true;
	}
};

