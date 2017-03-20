#include "RoomChannelManager.h"
#include "RoomHandler.h"

CRoomChannelManager::CRoomChannelManager() :
	Channels(new ChannelList),
	Rooms(new RoomList),
	CS_Room(new CRITICAL_SECTION()),
	CS_Channel(new CRITICAL_SECTION())
{
	InitializeCriticalSection(CS_Room);
	InitializeCriticalSection(CS_Channel);
}


CRoomChannelManager::~CRoomChannelManager()
{
}

RoomListIt CRoomChannelManager::getMyRoomIter(int ChannelNum, int roomNum)
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
	RoomListIt* error = nullptr;
	return *error;
}

CChannel * CRoomChannelManager::getMyChannel(int ChannelNum)
{
	ChannelListIt iterBegin = Channels->begin();
	ChannelListIt iterEnd = Channels->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->getChannelNum() == ChannelNum)
			return *iterBegin;
	}
	cout << ChannelNum << "번 채널이 없습니다." << endl;
	return nullptr;
}

