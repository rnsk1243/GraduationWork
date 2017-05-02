#include "RoomManager.h"
#include "RoomHandler.h"

CRoomManager::CRoomManager()
{
	//InitializeCriticalSection(&CS_Room);
}


CRoomManager::~CRoomManager()
{
	RoomListIt begin = getIterRoomBegin();
	RoomListIt end = getIterRoomEnd();
	for (; begin != end; ++begin)
	{
		delete(*begin);
	}
	Rooms.clear();
	//DeleteCriticalSection(&CS_Room);
}

RoomListIt CRoomManager::getMyRoomIter(int ChannelNum, int roomNum)
{
	RoomListIt iterBegin = Rooms.begin();
	RoomListIt iterEnd = Rooms.end();
	
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

int CRoomManager::getEmptyRoomNum()
{
	RoomListIt roomEnd = Rooms.end();
	--roomEnd;
	return ((*roomEnd)->getRoomNum() + 1);
}

