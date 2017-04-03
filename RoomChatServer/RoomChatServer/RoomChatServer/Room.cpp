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
	if (ClientInfos->empty())
	{
		cout << RoomNum << " 번 방이 삭제 됩니다." << endl;
		delete RoomName;
		LinkListIt begin = getIterMyInfoBegin();
		LinkListIt end = getIterMyInfoEnd();
		for (; begin != end; ++begin)
		{
			delete(*begin);
		}
		ClientInfos->clear();
		DeleteCriticalSection(CS_MyInfoList);
		cout << "방 삭제 완료" << endl;
	}else
	{
		cout << "방 삭제 실패" << endl;
	}
}

