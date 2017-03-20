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
		// ä���� ������ Ȯ��
		if (ChannelNum == (*iterBegin)->getChannelNum())
		{
			// ä���� ���� ���ȣ ���� ������ Ȯ��
			if (roomNum == (*iterBegin)->getRoomNum())
			{
				return iterBegin;
			}
		}	
	}
	cout << roomNum << "�� ���� �����ϴ�." << endl;
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
	cout << ChannelNum << "�� ä���� �����ϴ�." << endl;
	return nullptr;
}

