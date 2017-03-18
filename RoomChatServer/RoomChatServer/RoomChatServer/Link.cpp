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
//	// 방에도 안들어갔는데 방을 나온다고?!
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
//	// 방에 들어가 있으면 채널을 빠져나갈 수 없다 (방부터 나와라)
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
		// 채널이 같은지 확인
		if (ChannelNum == (*iterBegin)->getChannelNum())
		{
			// 채널이 같고 룸번호 까지 같은지 확인
			if (roomNum == (*iterBegin)->getRoomNum())
			{
				return iterBegin;
			}
		}	
	}
	cout << roomNum << "번 방이 없습니다." << endl;
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
	cout << ChannelNum << "번 채널이 없습니다." << endl;
	return nullptr;
}

