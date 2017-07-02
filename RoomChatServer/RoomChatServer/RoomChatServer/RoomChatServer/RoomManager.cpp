#include "RoomManager.h"
#include "RoomHandler.h"

CRoomManager::CRoomManager()
{
	//InitializeCriticalSection(&CS_Room);
}


CRoomManager::~CRoomManager()
{
	//RoomListIt begin = getIterRoomBegin();
	//RoomListIt end = getIterRoomEnd();
	//for (; begin != end; ++begin)
	//{
	//	delete(*begin);
	//}
	//Rooms.clear();
	//DeleteCriticalSection(&CS_Room);
}

RoomListIt CRoomManager::GetMyRoomIter(int ChannelNum, int roomNum)
{
	RoomListIt iterBegin = mRooms.begin();
	RoomListIt iterEnd =mRooms.end();
	
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		// ä���� ������ Ȯ��
		if (ChannelNum == (*iterBegin)->GetChannelNum())
		{
			// ä���� ���� ���ȣ ���� ������ Ȯ��
			if (roomNum == (*iterBegin)->GetRoomNum())
			{
				return iterBegin;
			}
		}	
	}
	cout << roomNum << "�� ���� �����ϴ�." << endl;
	//RoomListIt* error = nullptr;
	return iterBegin; // iterBegin == iterEnd �̸� ���̾���.
}

int CRoomManager::GetEmptyRoomNum()
{
	RoomListIt roomEnd = mRooms.end();
	--roomEnd;
	return ((*roomEnd)->GetRoomNum() + 1);
}

