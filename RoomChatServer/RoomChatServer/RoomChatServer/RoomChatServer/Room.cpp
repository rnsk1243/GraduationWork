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
	cout << mRoomNum << " �� ���� ���� �˴ϴ�." << endl;
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
	//	cout << "�� ���� ����" << endl;
	//}
	cout << "�� ���� �Ϸ�" << endl;
}

