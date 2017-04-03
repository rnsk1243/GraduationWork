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
	//RoomListIt* error = nullptr;
	return iterBegin; // iterBegin == iterEnd �̸� ���̾���.
}

