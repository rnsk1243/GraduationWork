#include "CommandController.h"



CCommandController::CCommandController(CChannelManager& channelManager, CRoomManager& roomManager, CChannelHandler& channelHandler, CRoomHandler& roomHandler)
	:ChannelManager(channelManager),
	RoomManager(roomManager), 
	ChannelHandler(channelHandler), 
	RoomHandler(roomHandler)
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
}


int CCommandController::commandHandling(CLink& clientInfo, char * command)
{
	if (command == nullptr)
		return OccuredError;
	cout << "명령 처리 시작" << endl;
	int channelNum = clientInfo.getMyChannelNum();
	int roomNum = clientInfo.getMyRoomNum();
	command++;
#pragma region 명령처리
	if (*command == 'e') // 방에 입장
	{
		bool isEnterSucces = false;
#pragma region 개설된 모든 방
		RoomListIt roomBegin = RoomManager.getIterRoomBegin();
		RoomListIt roomEnd = RoomManager.getIterRoomEnd();
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
					RoomHandler.enterRoom(&clientInfo, &RoomManager, room->getRoomNum());
					// 채널에서는 나가기
					ChannelHandler.exitChannel(&clientInfo, &ChannelManager);
					isEnterSucces = true;
					break;
				}
			}
		}
#pragma endregion
		if(!isEnterSucces)
			cout << "입장 할 수 있는 방이 없습니다." << endl;
	}
	else if (*command == 'c')
	{
		if(NoneRoom != clientInfo.getMyRoomNum())
			return SuccesCommand;
		ChannelHandler.exitChannel(&clientInfo, &ChannelManager);
		if (channelNum == MaxChannelNum)
		{
			ChannelHandler.enterChannel(&clientInfo, &ChannelManager, EnterChannelNum);
			return SuccesCommand;
		}
		ChannelListIt channelBegin = ChannelManager.getIterChannelBegin(); // const iterator로 바꿈
		ChannelListIt channelEnd = ChannelManager.getIterChannelEnd();
		for (; channelBegin != channelEnd; ++channelBegin)
		{
			if ((*channelBegin)->getChannelNum() == channelNum)
			{
				++channelBegin;
				int moveChannelNum = (*channelBegin)->getChannelNum();
				ChannelHandler.enterChannel(&clientInfo, &ChannelManager, moveChannelNum);
				cout << moveChannelNum << "번 채널 변경" << endl;
				break;
			}
		}
	}
	else if (*command == 'm')
	{
		//원래 채널에서는 나가기
		ChannelHandler.exitChannel(&clientInfo, &ChannelManager);
		cout << "방 만들기" << endl;
		char* roomName = RoomHandler.returnRoomName(command);
		RoomHandler.makeRoom(&clientInfo, &RoomManager, roomName);
	}
	else if (*command == 'o')
	{
		// 다시 채널로 돌아가고
		ChannelHandler.enterChannel(&clientInfo, &ChannelManager, channelNum);
		cout << "방에서 나가기" << endl;
		RoomHandler.exitRoom(&clientInfo, &RoomManager);

	}
	else if (*command == 'i')
	{
#pragma region 풀방까지 몇명 필요?
		CRoom* myRoom = *RoomManager.getMyRoomIter(channelNum, roomNum);
		// 풀방까지 몇명 필요한가? (제한인원 - 현재 방 인원)
		int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->getAmountPeople());
#pragma endregion
		// 합칠 대상 방 검색
		RoomListIt roomListBegin = RoomManager.getIterRoomBegin();
		RoomListIt roomListEnd = RoomManager.getIterRoomEnd();
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
					RoomManager.eraseRoom(roomListBegin); // 합칠 대상 방 리스트에서 제거
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
		clientInfo.changeName(command, 1);
		cout << clientInfo.getMyName() << " 으로 이름 변경 됨" << endl;
	}
#pragma endregion
	return SuccesCommand;
}

bool CCommandController::deleteClientSocket(CLink& clientInfo)
{
	int myChannelNum = clientInfo.getMyChannelNum();
	int myRoomNum = clientInfo.getMyRoomNum();
	//방에 있나 채널에 있나 확인
	if (NoneRoom == myRoomNum)
	{
		// 채널일때
		return ChannelHandler.exitChannel(&clientInfo, &ChannelManager);
	}
	else
	{
		// 방일때
		return RoomHandler.exitRoom(&clientInfo, &RoomManager);
	}
	return false;
}