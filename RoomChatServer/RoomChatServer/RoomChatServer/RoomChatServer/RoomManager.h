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
	RoomList mRooms;
#pragma region ũ��Ƽ�� ����
//	CRITICAL_SECTION CS_Room;
	MUTEX mRAII_RoomManagerMUTEX;
#pragma endregion
public:
	CRoomManager();
	CRoomManager(const CRoomManager&) = delete;
	CRoomManager& operator=(const CRoomManager&) = delete;
	~CRoomManager();
#pragma region push, erase �޼ҵ�

	void PushRoom(shared_ptr<CRoom> shared_newRoom)
	{
		if (0 >= shared_newRoom.use_count())
		{
			CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
			return;
		}
		ScopeLock<MUTEX> MU(mRAII_RoomManagerMUTEX);
		mRooms.push_back(shared_newRoom);
	}
	RoomListIt EraseRoom(RoomListIt delRoom)
	{
		ScopeLock<MUTEX> MU(mRAII_RoomManagerMUTEX);
		RoomListIt temp;
		temp = mRooms.erase(delRoom);
		return temp;
	}
#pragma endregion

#pragma region get �޼ҵ�
	RoomListIt GetMyRoomIter(int ChannelNum, int roomNum);
	RoomListIt GetIterRoomBegin() { return mRooms.begin(); }
	RoomListIt GetIterRoomEnd() { return mRooms.end(); }
	int GetEmptyRoomNum();
#pragma endregion
	bool IsRoomListEmpty() { return mRooms.empty(); }
};
 
