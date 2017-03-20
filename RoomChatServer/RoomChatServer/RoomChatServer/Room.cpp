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
	cout << RoomNum << " 번 방이 삭제 됩니다." << endl;
	// delete &ChannelNum; 지울 필요가 없나요?
	delete RoomName;
	delete ClientInfos;
	DeleteCriticalSection(CS_MyInfoList);
	cout << "방 삭제 완료" << endl;
}

