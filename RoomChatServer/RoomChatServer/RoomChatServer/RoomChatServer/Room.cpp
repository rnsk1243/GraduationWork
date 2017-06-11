#include "Room.h"



CRoom::CRoom(int roomNum, int channelNum, char* roomName) :
	RoomNum(roomNum),
	ChannelNum(channelNum),
	RoomName(roomName),
	AmountPeople(0)
{
	
	//InitializeCriticalSection(&CS_MyInfoList);
}


CRoom::~CRoom()
{
	cout << RoomNum << " 번 방이 삭제 됩니다." << endl;
	delete[] RoomName;
	//if (ClientInfos.empty())
	//{
	//	LinkListIt begin = getIterMyInfoBegin();
	//	LinkListIt end = getIterMyInfoEnd();
	//	for (; begin != end; ++begin)
	//	{
	//		delete(*begin);
	//	}
	//	ClientInfos.clear();
	//	//DeleteCriticalSection(&CS_MyInfoList);
	//}else
	//{
	//	cout << "방 삭제 실패" << endl;
	//}
	cout << "방 삭제 완료" << endl;
}

