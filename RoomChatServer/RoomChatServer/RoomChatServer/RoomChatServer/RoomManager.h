#pragma once
#include<list>
#include<iostream>
#include"Room.h"
#include"ErrorHandler.h"
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
#pragma endregion
public:
	CRoomManager();
	CRoomManager(const CRoomManager&) = delete;
	CRoomManager& operator=(const CRoomManager&) = delete;
	~CRoomManager();
#pragma region push, erase �޼ҵ�

	void pushRoom(shared_ptr<CRoom> shared_newRoom)
	{
		if (0 >= shared_newRoom.use_count())
		{
			CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
			return;
		}
		ScopeLock<MUTEX> MU(RAII_RoomManagerMUTEX);
		Rooms.push_back(shared_newRoom);
	}
	RoomListIt eraseRoom(RoomListIt delRoom)
	{
		ScopeLock<MUTEX> MU(RAII_RoomManagerMUTEX);
		RoomListIt temp;
		temp = Rooms.erase(delRoom);
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
 
