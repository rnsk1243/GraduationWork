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
	cout << RoomNum << " 번 방이 삭제 됩니다." << endl;
	// delete &ChannelNum; 지울 필요가 없나요?
	delete RoomName;
	delete RoomSocketList;
	DeleteCriticalSection(CS_RoomSocketList);
	cout << "방 삭제 완료" << endl;
}

