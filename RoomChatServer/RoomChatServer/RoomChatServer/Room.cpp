#include "Room.h"



CRoom::CRoom(int roomNo, int channelNum, char* roomName):AmountPeople(0)
{
	RoomSocketList = new SocketList();
	CS_RoomSocketList = new CRITICAL_SECTION();
	InitializeCriticalSection(CS_RoomSocketList);
	ChannelNum = channelNum;
	RoomName = roomName;
	RoomNum = roomNo;
}


CRoom::~CRoom()
{
	cout << RoomNum << " �� ���� ���� �˴ϴ�." << endl;
	// delete &ChannelNum; ���� �ʿ䰡 ������?
	delete RoomName;
	delete RoomSocketList;
	DeleteCriticalSection(CS_RoomSocketList);
	cout << "�� ���� �Ϸ�" << endl;
}

