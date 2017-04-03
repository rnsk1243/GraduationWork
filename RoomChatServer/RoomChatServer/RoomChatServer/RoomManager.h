#pragma once
#include<list>
#include<iostream>
#include"Room.h"

class CLink;
class CRoom;
using namespace std;
#pragma region 타입재정의
typedef list<CRoom*> RoomList;
typedef RoomList::iterator RoomListIt;
#pragma endregion

class CRoomManager
{
	RoomList* Rooms;
#pragma region 크리티컬 섹션
	CRITICAL_SECTION* CS_Room;
	
#pragma endregion
public:
	CRoomManager();
	~CRoomManager();
#pragma region push, erase 메소드

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

#pragma region get 메소드
	RoomListIt getMyRoomIter(int ChannelNum, int roomNum);
	RoomListIt getIterRoomBegin() { return Rooms->begin(); }
	RoomListIt getIterRoomEnd() { return Rooms->end(); }
#pragma endregion
	bool isRoomListEmpty() { return Rooms->empty(); }
};
 
