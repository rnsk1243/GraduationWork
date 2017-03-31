#include "Room.h"



CRoom::CRoom(int roomNum, int channelNum, char* roomName) :
	RoomNum(roomNum),
	ChannelNum(channelNum),
	RoomName(roomName),
	AmountPeople(0)
{
	InitializeCriticalSection(&CS_MyInfoList);
}


CRoom::~CRoom()
{
	if (ClientInfos.empty())
	{
		cout << RoomNum << " �� ���� ���� �˴ϴ�." << endl;
		delete RoomName;
		DeleteCriticalSection(&CS_MyInfoList);
		cout << "�� ���� �Ϸ�" << endl;
	}else
	{
		cout << "�� ���� ����" << endl;
	}
}

