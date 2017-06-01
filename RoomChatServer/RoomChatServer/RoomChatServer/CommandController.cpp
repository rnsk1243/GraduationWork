#define _CRT_SECURE_NO_WARNINGS
#include "CommandController.h"
#include"ConstEnumInfo.h"

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
}


int CCommandController::commandHandling(CLink& clientInfo,const string& command, g_Message& sendServerMessage)
{
	if (&command == nullptr)
		return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	cout << "명령 처리 시작" << endl;
	cout << "받은 메시지 = " << command << endl;
	int channelNum = clientInfo.getMyChannelNum();
	int roomNum = clientInfo.getMyRoomNum();
#pragma region 명령처리
	if (0 == command.compare("EnterRoom")) // 방에 입장
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
					ChannelHandler.exitChannel(clientInfo, ChannelManager);
					isEnterSucces = true;
					string roomName = room->getRoomName();
					int amount = room->getAmountPeople();
					char amoutChar[10];
					_itoa(amount, amoutChar, 10);
					string roomAountPeople = amoutChar;
					if (EnterRoomPeopleLimit > amount)
					{
						sendServerMessage.set_message(roomName + " Romm In // current room amount = " + roomAountPeople);
					}
					else if (EnterRoomPeopleLimit == amount)
					{
						sendServerMessage.set_message("all Player!");
						return SUCCES_PLAYER_AMOUNT;
					}
					break;
				}
			}
		}
#pragma endregion
		if (!isEnterSucces)
		{
			cout << "입장 할 수 있는 방이 없습니다." << endl;
			ChannelHandler.exitChannel(clientInfo, ChannelManager);
			cout << "방 만들기" << endl;
			char roomName[RoomNameSize];
			const string roomNameStr = "room";
			strcpy_s(roomName, RoomNameSize, roomNameStr.c_str());
			cout << "만들고자 하는 방 이름 = " << roomName << endl;
			RoomHandler.makeRoom(&clientInfo, &RoomManager, roomName);
			sendServerMessage.set_message("입장할 수 있는 방이 없어서 " + roomNameStr + "으로 방 만들기 성공 하셨습니다.");
		}
	}
	else if (0 == command.compare("NextChannel"))
	{
		if(NoneRoom != clientInfo.getMyRoomNum())
			return SUCCES_COMMAND;
		ChannelHandler.exitChannel(clientInfo, ChannelManager);
		if (channelNum == MaxChannelNum)
		{
			ChannelHandler.enterChannel(&clientInfo, ChannelManager, EnterChannelNum);
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
				ChannelHandler.enterChannel(&clientInfo, ChannelManager, moveChannelNum);
				cout << moveChannelNum << "번 채널 변경" << endl;
				sendServerMessage.set_message(moveChannelNum + " Channel In!");
				break;
			}
		}
		
	}
	else if (0 == command.compare("MakeRoom"))
	{
		//원래 채널에서는 나가기
		ChannelHandler.exitChannel(clientInfo, ChannelManager);
		cout << "방 만들기" << endl;
		char roomName[RoomNameSize];
		strcpy_s(roomName, RoomNameSize, command.c_str());
		cout << "만들고자 하는 방 이름 = " << roomName << endl;
		RoomHandler.makeRoom(&clientInfo, &RoomManager, roomName);
		string name = roomName;
		sendServerMessage.set_message(name + " room Create!");
	}
	else if (0 == command.compare("OutRoom"))
	{
		// 다시 채널로 돌아가고
		ChannelHandler.enterChannel(&clientInfo, ChannelManager, channelNum);
		cout << "방에서 나가기" << endl;
		RoomHandler.exitRoom(&clientInfo, &RoomManager);
		sendServerMessage.set_message("Out Room!");
	}
	else if (0 == command.compare("MergeRoom"))
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
				sendServerMessage.set_message("방 합체 완료");
				break; // 가장 먼저 검색되는 아무 방과 병합 후 빠져나옴
			}
		}
		if (!isMergeSucces)
			cout << "방 merge 실패" << endl;
	}
	else if (0 == command.compare("servantTofu"))
	{
		clientInfo.setMyServentNumber(servantTofu);
		cout << "이 클라이언트의 서번트 = 두부" << endl;
		sendServerMessage.set_message("My Servant Tofu");
	}
	else if (0 == command.compare("servantMandu"))
	{
		clientInfo.setMyServentNumber(servantMando);
		
		cout << "이 클라이언트의 서번트 = 만두" << endl;
		sendServerMessage.set_message("My Servant Mandu");
	}
	else if (0 == command.compare("teamRed"))
	{
		clientInfo.setMyTeam(RedTeam);

		cout << "이 클라이언트의 팀 = 레드" << endl;
		sendServerMessage.set_message("My Team Red");
	}
	else if (0 == command.compare("teamBlue"))
	{
		clientInfo.setMyTeam(BlueTeam);

		sendServerMessage.set_message("My Team Blue");
	}
	else if (0 == command.compare("start"))
	{
		CRoom* myRoom = *(RoomManager.getMyRoomIter(channelNum, roomNum));
		if (myRoom == nullptr)
		{
			sendServerMessage.set_message("My Room None");
			return CErrorHandler::ErrorHandler(ERROR_ROOM_NONE);
		}
		bool isTeamValance = false;
		bool isPlayerInfoLack = true;
		if (myRoom->isTeamBalance())
		{
			isTeamValance = true;
		}
		if (myRoom->updatePlayerReadyInfo())
		{
			isPlayerInfoLack = false;
		}
		if (isTeamValance && !isPlayerInfoLack)
		{
			sendServerMessage.set_message("Game Set All Ready! Start!");
			return SUCCES_PLAYER_INFO_LACK;
		}

		if (!isTeamValance)
		{
			sendServerMessage.set_message("Team Valance No Ready");
			return ERROR_TEAM_VALANCE;
		}
		sendServerMessage.set_message("Player Info Lack!");
		return ERROR_PLAYER_INFO_LACK;
	}
	//else if (*command == 'n')
	//{
	//	// 기존 이름 변경
	//	clientInfo.changeName(command, 1);
	//	cout << clientInfo.getMyName() << " 으로 이름 변경 됨" << endl;
	//}
#pragma endregion
	return SUCCES_COMMAND;
}

bool CCommandController::deleteClientSocket(CLink& clientInfo)
{
	int myChannelNum = clientInfo.getMyChannelNum();
	int myRoomNum = clientInfo.getMyRoomNum();
	//방에 있나 채널에 있나 확인
	if (NoneRoom == myRoomNum)
	{
		// 채널일때
		return ChannelHandler.exitChannel(clientInfo, ChannelManager);
	}
	else
	{
		// 방일때
		return RoomHandler.exitRoom(&clientInfo, &RoomManager);
	}
	return false;
}