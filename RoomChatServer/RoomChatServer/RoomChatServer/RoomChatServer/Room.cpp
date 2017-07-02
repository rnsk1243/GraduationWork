#include "Room.h"



CRoom::CRoom(int roomNum, int channelNum, const string& roomName) :
	mRoomNum(roomNum),
	mChannelNum(channelNum),
	mRoomName(roomName),
	mAmountPeople(0)
{
	
	//InitializeCriticalSection(&CS_MyInfoList);
}


CRoom::~CRoom()
{
	cout << mRoomNum << " 번 방이 삭제 됩니다." << endl;
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

