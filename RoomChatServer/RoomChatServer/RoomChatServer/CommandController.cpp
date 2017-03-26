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
	else if (*command == 'i')
	{
#pragma region 풀방까지 몇명 필요?
		CRoom* myRoom = *RoomChannelManager->getMyRoomIter(channelNum, roomNum);
		// 풀방까지 몇명 필요한가? (제한인원 - 현재 방 인원)
		int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->getAmountPeople());
#pragma endregion
		// 합칠 대상 방 검색
		RoomListIt roomListBegin = RoomChannelManager->getIterRoomBegin();
		RoomListIt roomListEnd = RoomChannelManager->getIterRoomEnd();
		bool isMergeSucces = false;
		for (; roomListBegin != roomListEnd; ++roomListBegin)
		{
			if (roomNum == (*roomListBegin)->getRoomNum())
			{
				cout << "나 자신 방" << endl;
				continue;
			}
			if ((*roomListBegin)->getAmountPeople() <= limitToPeopleNum)
			{
				CRoom* targetRoom = (*roomListBegin);
				if (myRoom->mergeRoom(targetRoom))
				{
					RoomChannelManager->eraseRoom(roomListBegin); // 합칠 대상 방 리스트에서 제거
					delete targetRoom; // 방 제거
				}
				cout << "방 합체 완료" << endl; isMergeSucces = true;
				break; // 가장 먼저 검색되는 아무 방과 병합 후 빠져나옴
			}
		}
		if (!isMergeSucces)
			cout << "방 merge 실패" << endl;
	}
	else if (*command == 'n')
	{
		// 기존 이름 변경
		clientInfo->changeName(command, 1);
		cout << clientInfo->getMyName() << " 으로 이름 변경 됨" << endl;
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