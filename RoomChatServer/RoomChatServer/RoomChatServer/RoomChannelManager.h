#pragma once
#include<list>
#include<iostream>
#include"Channel.h"
#include"Room.h"
class CChannel;
class CLink;
class CRoom;
using namespace std;
#pragma region Ÿ��������
typedef list<CChannel*> ChannelList;
typedef ChannelList::iterator ChannelListIt;
typedef list<CRoom*> RoomList;
typedef RoomList::iterator RoomListIt;
#pragma endregion

class CRoomChannelManager
{
	ChannelList* Channels;
	RoomList* Rooms;
#pragma region ũ��Ƽ�� ����
	CRITICAL_SECTION* CS_Room;
	CRITICAL_SECTION* CS_Channel;
#pragma endregion
public:
	CRoomChannelManager();
	~CRoomChannelManager();
#pragma region push, erase �޼ҵ�
	void pushChannel(CChannel* newChannel)
	{
		EnterCriticalSection(CS_Channel);
		Channels->push_front(newChannel);
		LeaveCriticalSection(CS_Channel);
	}
	void pushRoom(CRoom* newRoom)
	{
		EnterCriticalSection(CS_Room);
		Rooms->push_front(newRoom);
		LeaveCriticalSection(CS_Room);
	}
	RoomListIt eraseRoom(RoomListIt delRoom)
	{
		RoomListIt temp;
		EnterCriticalSection(CS_Room);
		temp = Rooms->erase(delRoom);
		LeaveCriticalSection(CS_Room);
		return temp;
	}
#pragma endregion

#pragma region get �޼ҵ�
	RoomListIt getMyRoomIter(int ChannelNum, int roomNum);
	CChannel* getMyChannel(int ChannelNum);
	ChannelListIt getIterChannelBegin() { return Channels->begin(); }
	ChannelListIt getIterChannelEnd() { return Channels->end(); }
	RoomListIt getIterRoomBegin() { return Rooms->begin(); }
	RoomListIt getIterRoomEnd() { return Rooms->end(); }
#pragma endregion
	bool isRoomListEmpty() { return Rooms->empty(); }
};
 
