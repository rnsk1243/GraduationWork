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
	cout << RoomNum << " �� ���� ���� �˴ϴ�." << endl;
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
	//	cout << "�� ���� ����" << endl;
	//}
	cout << "�� ���� �Ϸ�" << endl;
}

