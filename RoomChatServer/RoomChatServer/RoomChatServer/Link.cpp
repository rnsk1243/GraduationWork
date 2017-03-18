#include "Link.h"
#include "RoomHandlere.h"

CLink::CLink()
{
	Rooms = new RoomList();
	Channels = new ChannelList();
	CS_Room = new CRITICAL_SECTION();
	CS_Channel = new CRITICAL_SECTION();
	InitializeCriticalSection(CS_Room);
	InitializeCriticalSection(CS_Channel);
}


CLink::~CLink()
{
}

//bool CLink::eraseRoomSocket(CMyInfo * clientInfo)
//{
//	// �濡�� �ȵ��µ� ���� ���´ٰ�?!
//	if (clientInfo->getMyChannelNum() == NoneRoom)
//		return false;
//	int myChannelNum = clientInfo->getMyChannelNum();
//	int myRoomNum = clientInfo->getMyRoomNum();
//	CRoom* myRoom = (*getMyRoomIter(myChannelNum, myRoomNum));
//
//	SocketListIt roomSocketIterBegin = myRoom->getIterSocketBegin();
//	SocketListIt roomSocketIterEnd = myRoom->getIterSocketEnd();
//
//	for (; roomSocketIterBegin != roomSocketIterEnd; ++roomSocketIterBegin)
//	{
//		if ((*roomSocketIterBegin) == clientInfo->getClientSocket())
//		{
//			myRoom->eraseSocket(roomSocketIterBegin);
//			return true;
//		}
//	}
//	return false;
//}
//
//bool CLink::eraseChannelSocket(CMyInfo * clientInfo)
//{
//	// �濡 �� ������ ä���� �������� �� ���� (����� ���Ͷ�)
//	if (clientInfo->getMyChannelNum() != NoneRoom)
//		return false;
//	int myChannelNum = clientInfo->getMyChannelNum();
//	int myRoomNum = clientInfo->getMyRoomNum();
//	CChannel* myChannel = getMyChannel(myChannelNum);
//	SocketListIt channelSocketIterBegin = myChannel->getIterBegin();
//	SocketListIt channelSocketIterEnd = myChannel->getIterEnd();
//
//	for (; channelSocketIterBegin != channelSocketIterEnd; ++channelSocketIterBegin)
//	{
//		if ((*channelSocketIterBegin) == clientInfo->getClientSocket())
//		{
//			myChannel->eraseSocket(channelSocketIterBegin);
//			return true;
//		}
//	}
//	return false;
//}

RoomListIt CLink::getMyRoomIter(int ChannelNum, int roomNum)
{
	RoomListIt iterBegin = Rooms->begin();
	RoomListIt iterEnd = Rooms->end();
	
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		// ä���� ������ Ȯ��
		if (ChannelNum == (*iterBegin)->getChannelNum())
		{
			// ä���� ���� ���ȣ ���� ������ Ȯ��
			if (roomNum == (*iterBegin)->getRoomNum())
			{
				return iterBegin;
			}
		}	
	}
	cout << roomNum << "�� ���� �����ϴ�." << endl;
	RoomListIt* error = nullptr;
	return *error;
}

CChannel * CLink::getMyChannel(int ChannelNum)
{
	ChannelListIt iterBegin = Channels->begin();
	ChannelListIt iterEnd = Channels->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->getChannelNum() == ChannelNum)
			return *iterBegin;
	}
	cout << ChannelNum << "�� ä���� �����ϴ�." << endl;
	return nullptr;
}

