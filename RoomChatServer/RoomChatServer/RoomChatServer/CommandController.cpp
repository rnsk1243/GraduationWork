#include "CommandController.h"
#include"ErrorHandler.h"

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
}


int CCommandController::commandHandling(shared_ptr<CLink> shared_clientInfo, char * command)
{
	CLink* clientInfo = nullptr;
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
	}
	else
	{
		return CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
	}
	if (command == nullptr)
		return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	cout << "명령 처리 시작" << endl;
	int channelNum = clientInfo->getMyChannelNum();
	int roomNum = clientInfo->getMyRoomNum();
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
			CRoom* room = (*roomBegin).get();
			if (room->getChannelNum() == channelNum)
			{
				if (room->getAmountPeople() < EnterRoomPeopleLimit)
				{
					cout << "방에 입장" << endl;
					if (!RoomHandler.enterRoom(shared_clientInfo, &RoomManager, room->getRoomNum()))
					{
						return CErrorHandler::ErrorHandler(ERROR_ENTER_ROOM);
					}
					// 채널에서는 나가기
					if(!ChannelHandler.exitChannel(*clientInfo, ChannelManager))
						return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
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
		if(NoneRoom != clientInfo->getMyRoomNum())
			return CErrorHandler::ErrorHandler(ERROR_COMMAND);
		if (!ChannelHandler.exitChannel(*clientInfo, ChannelManager))
			return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
		if (channelNum == MaxChannelNum)
		{
			if (!ChannelHandler.enterChannel(shared_clientInfo, ChannelManager, EnterChannelNum))
			{
				return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
			}
			return SUCCES_COMMAND;
		}
		ChannelListIt channelBegin = ChannelManager.getIterChannelBegin(); // const iterator로 바꿈
		ChannelListIt channelEnd = ChannelManager.getIterChannelEnd();
		for (; channelBegin != channelEnd; ++channelBegin)
		{
			if ((*channelBegin)->getChannelNum() == channelNum)
			{
				++channelBegin;
				int moveChannelNum = (*channelBegin)->getChannelNum();
				if (!ChannelHandler.enterChannel(shared_clientInfo, ChannelManager, moveChannelNum))
				{
					return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
				}
				cout << moveChannelNum << "번 채널 변경" << endl;
				break;
			}
		}
	}
	else if (*command == 'm')
	{
		cout << "방 만들기" << endl;
		char* roomName = RoomHandler.returnRoomName(command);
		if (!RoomHandler.makeRoom(shared_clientInfo, &RoomManager, roomName))
		{
			return CErrorHandler::ErrorHandler(ERROR_MAKE_ROOM);
		}
		//원래 채널에서는 나가기
		if (!ChannelHandler.exitChannel(*clientInfo, ChannelManager))
			return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
	}
	else if (*command == 'o')
	{
		// 다시 채널로 돌아가고
		if (!ChannelHandler.enterChannel(shared_clientInfo, ChannelManager, channelNum))
		{
			return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
		}
		cout << "방에서 나가기" << endl;
		if (!RoomHandler.exitRoom(clientInfo, &RoomManager))
			return CErrorHandler::ErrorHandler(ERROR_EXIT_ROOM);

	}
	else if (*command == 'i')
	{
#pragma region 풀방까지 몇명 필요?
		CRoom* myRoom = (*RoomManager.getMyRoomIter(channelNum, roomNum)).get();
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
				if (0 >= (*roomListBegin).use_count())
				{
					return CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
				}
				CRoom* targetRoom = (*roomListBegin).get();
				if (myRoom->mergeRoom(targetRoom))
				{
					RoomManager.eraseRoom(roomListBegin); // 합칠 대상 방 리스트에서 제거
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
	return SUCCES_COMMAND;
}

bool CCommandController::deleteClientSocket(CLink& clientInfo)
{
	cout << "지울 소켓 이름 = " << clientInfo.getMyName() << endl;
	int myChannelNum = clientInfo.getMyChannelNum();
	int myRoomNum = clientInfo.getMyRoomNum();
	//방에 있나 채널에 있나 확인
	if (NoneRoom == myRoomNum)
	{
		// 채널일때
		if (!ChannelHandler.exitChannel(clientInfo, ChannelManager))
		{
			CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
			return false;
		}
		else 
		{ 
			return true; 
		}
	}
	else
	{
		// 방일때
		if (!RoomHandler.exitRoom(&clientInfo, &RoomManager))
		{
			CErrorHandler::ErrorHandler(ERROR_EXIT_ROOM);
			return false;
		}
		else 
		{
			return true;
		}
	}
	return false;
}