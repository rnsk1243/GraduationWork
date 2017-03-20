#include "Room.h"



CRoom::CRoom(int roomNum, int channelNum, char* roomName) :
	RoomNum(roomNum),
	ChannelNum(channelNum),
	RoomName(roomName),
	AmountPeople(0),
	ClientInfos(new LinkList()),
	CS_MyInfoList(new CRITICAL_SECTION)
{
	InitializeCriticalSection(CS_MyInfoList);
}


CRoom::~CRoom()
{
	cout << RoomNum << " �� ���� ���� �˴ϴ�." << endl;
	// delete &ChannelNum; ���� �ʿ䰡 ������?
	delete RoomName;
	delete ClientInfos;
	DeleteCriticalSection(CS_MyInfoList);
	cout << "�� ���� �Ϸ�" << endl;
}

