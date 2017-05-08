#pragma once
#include<list>
#include<iostream>
#include"Room.h"

class CLink;
class CRoom;
using namespace std;
#pragma region Ÿ��������
typedef list<shared_ptr<CRoom>> RoomList;
typedef RoomList::iterator RoomListIt;
#pragma endregion

class CRoomManager
{
	RoomList Rooms;
#pragma region ũ��Ƽ�� ����
//	CRITICAL_SECTION CS_Room;
	MUTEX RAII_RoomManagerMUTEX;
	CRoomManager(const CRoomManager&);
	CRoomManager& operator=(const CRoomManager&);
#pragma endregion
public:
	CRoomManager();
	~CRoomManager();
#pragma region push, erase �޼ҵ�

	void pushRoom(shared_ptr<CRoom> newRoom)
	{
		{
			ScopeLock<MUTEX> MU(RAII_RoomManagerMUTEX);
			Rooms.push_back(newRoom);
		}
	}
	RoomListIt eraseRoom(RoomListIt delRoom)
	{
		RoomListIt temp;
		{
			ScopeLock<MUTEX> MU(RAII_RoomManagerMUTEX);
			temp = Rooms.erase(delRoom);
		}
		return temp;
	}
#pragma endregion

#pragma region get �޼ҵ�
	RoomListIt getMyRoomIter(int ChannelNum, int roomNum);
	RoomListIt getIterRoomBegin() { return Rooms.begin(); }
	RoomListIt getIterRoomEnd() { return Rooms.end(); }
	int getEmptyRoomNum();
#pragma endregion
	bool isRoomListEmpty() { return Rooms.empty(); }
};
 
