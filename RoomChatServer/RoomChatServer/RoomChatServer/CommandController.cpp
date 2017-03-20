#include "CommandController.h"



CCommandController::CCommandController(CRoomChannelManager* roomChannelManager, CChannelHandler* channelHandler, CRoomHandler* roomHandler)
	:RoomChannelManager(roomChannelManager), 
	ChannelHandler(channelHandler), 
	RoomHandlere(roomHandler)
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
}


int CCommandController::commandHandling(CLink* clientInfo, char * command)
{
	if (command == nullptr)
		return OccuredError;
	cout << "명령 처리 시작" << endl;
	int channelNum = clientInfo->getMyChannelNum();
	int roomNum = clientInfo->getMyRoomNum();
	*command++;
#pragma region 명령처리
	if (*command == 'e') // 방에 입장
	{
		bool isEnterSucces = false;
#pragma region 개설된 모든 방
		RoomListIt roomBegin = RoomChannelManager->getIterRoomBegin();
		RoomListIt roomEnd = RoomChannelManager->getIterRoomEnd();
#pragma endregion
#pragma region 내가 입장 할 수 있는 방 찾기
		for (; roomBegin != roomEnd; ++roomBegin)
		{
			CRoom* room = *roomBegin;
			if (room->getChannelNum() == channelNum)
			{
				if (room->getAmountPeople() < EnterRoomPeopleLimit)
				{
					cout << "방에 입장" << endl;
					RoomHandlere->enterRoom(clientInfo, RoomChannelManager, room->getRoomNum());
					// 채널에서는 나가기
					ChannelHandler->exitChannel(clientInfo, RoomChannelManager);
					isEnterSucces = true;
				}
			}
		}
#pragma endregion
		if(!isEnterSucces)
			cout << "입장 할 수 있는 방이 없습니다." << endl;
	}
	else if (*command == 'c')
	{
		if(NoneRoom != clientInfo->getMyRoomNum())
			return SuccesCommand;
		ChannelHandler->exitChannel(clientInfo, RoomChannelManager);
		if (channelNum == MaxChannelNum)
		{
			ChannelHandler->enterChannel(clientInfo, RoomChannelManager, 0);
			return SuccesCommand;
		}
		ChannelListIt channelBegin = RoomChannelManager->getIterChannelBegin();
		ChannelListIt channelEnd = RoomChannelManager->getIterChannelEnd();
		for (; channelBegin != channelEnd; ++channelBegin)
		{
			if ((*channelBegin)->getChannelNum() == channelNum)
			{
				++channelBegin;
				int moveChannelNum = (*channelBegin)->getChannelNum();
				ChannelHandler->enterChannel(clientInfo, RoomChannelManager, moveChannelNum);
				cout << moveChannelNum << "번 채널 변경" << endl;
			}
		}
	}
	else if (*command == 'm')
	{
		//원래 채널에서는 나가기
		ChannelHandler->exitChannel(clientInfo, RoomChannelManager);
		cout << "방 만들기" << endl;
		char* roomName = RoomHandlere->returnRoomName(command);
		RoomHandlere->makeRoom(clientInfo, RoomChannelManager, roomName);
	}
	else if (*command == 'o')
	{
		// 다시 채널로 돌아가고
		ChannelHandler->enterChannel(clientInfo, RoomChannelManager, channelNum);
		cout << "방에서 나가기" << endl;
		RoomHandlere->exitRoom(clientInfo, RoomChannelManager);

	}
#pragma endregion
	return SuccesCommand;
}

bool CCommandController::deleteClientSocket(CLink* clientInfo)
{
	int myChannelNum = clientInfo->getMyChannelNum();
	int myRoomNum = clientInfo->getMyRoomNum();
	//방에 있나 채널에 있나 확인
	if (NoneRoom == myRoomNum)
	{
		// 채널일때
		return ChannelHandler->exitChannel(clientInfo, RoomChannelManager);
	}
	else
	{
		// 방일때
		return RoomHandlere->exitRoom(clientInfo, RoomChannelManager);
	}
	return false;
}