#include "RoomManager.h"
#include "RoomHandler.h"

CRoomManager::CRoomManager() :
	Rooms(new RoomList),
	CS_Room(new CRITICAL_SECTION())
{
	InitializeCriticalSection(CS_Room);
}


CRoomManager::~CRoomManager()
{
}

RoomListIt CRoomManager::getMyRoomIter(int ChannelNum, int roomNum)
{
	RoomListIt iterBegin = Rooms->begin();
	RoomListIt iterEnd = Rooms->end();
	
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		// 채널이 같은지 확인
		if (ChannelNum == (*iterBegin)->getChannelNum())
		{
			// 채널이 같고 룸번호 까지 같은지 확인
			if (roomNum == (*iterBegin)->getRoomNum())
			{
				return iterBegin;
			}
		}	
	}
	cout << roomNum << "번 방이 없습니다." << endl;
	//RoomListIt* error = nullptr;
	return iterBegin; // iterBegin == iterEnd 이면 방이없다.
}

