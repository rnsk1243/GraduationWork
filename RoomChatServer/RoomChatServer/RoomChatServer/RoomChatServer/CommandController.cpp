#include "CommandController.h"
#include"ErrorHandler.h"
//#include"ConstEnumInfo.h"
//#include"GaChar.h"

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
}

int CCommandController::readyCommand(shared_ptr<CLink> shared_clientInfo, CLink*& clientInfo, int& channelNum)
{
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
		channelNum = clientInfo->getMyChannelNum();
	}
	else
	{
		return CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
	}
	return 0;
}

int CCommandController::enterRoom(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr; 
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);
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
				if (!ChannelHandler.exitChannel(*clientInfo, ChannelManager))
					return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
				isEnterSucces = true;
				break;
			}
		}
	}
#pragma endregion
	if (!isEnterSucces)
		cout << "입장 할 수 있는 방이 없습니다." << endl;
	return 0;
}

int CCommandController::changeChannel(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);
	if (NoneRoom != clientInfo->getMyRoomNum())
		return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	if (!ChannelHandler.exitChannel(*clientInfo, ChannelManager))
		return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
	if (channelNum == MaxChannelNum)
	{
		if (!ChannelHandler.moveNextChannel(shared_clientInfo, ChannelManager, EnterChannelNum))
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
			if (!ChannelHandler.moveNextChannel(shared_clientInfo, ChannelManager, moveChannelNum))
			{
				return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
			}
			cout << moveChannelNum << "번 채널 변경" << endl;
			break;
		}
	}
	return 0;
}

int CCommandController::makeRoom(const string& roomName, shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);
	cout << "방 만들기" << endl;
	if (!RoomHandler.makeRoom(shared_clientInfo, &RoomManager, roomName))
	{
		return CErrorHandler::ErrorHandler(ERROR_MAKE_ROOM);
	}
	//원래 채널에서는 나가기
	if (!ChannelHandler.exitChannel(*clientInfo, ChannelManager))
		return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
	return 0;
}

int CCommandController::outRoom(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);
	// 다시 채널로 돌아가고
	if (!ChannelHandler.moveNextChannel(shared_clientInfo, ChannelManager, channelNum))
	{
		return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
	}
	cout << "방에서 나가기" << endl;
	if (!RoomHandler.exitRoom(clientInfo, &RoomManager))
		return CErrorHandler::ErrorHandler(ERROR_EXIT_ROOM);
	return 0;
}

int CCommandController::mergeRoom(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);

	int roomNum = clientInfo->getMyRoomNum();
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
	return 0;
}

int CCommandController::cardCompose(shared_ptr<CLink> shared_clientInfo, const string& targetCardNum, const string& sourceCardNum, MessageStruct* sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);

	int targetCardNumInt = stoi(targetCardNum);
	int sourceCardNumInt = stoi(sourceCardNum);
	int resultCompose = mCardManager.ComposeCard(*clientInfo, targetCardNumInt, sourceCardNumInt);
	if (ERROR_COMPOSE_EVOUTION_CARD == resultCompose)
	{
		sendClientMessage->message = "이 카드는 각성 해야 합성 할 수 있습니다.";
	}
	else if(ERROR_COMPOSE_NULL_CARD == resultCompose)
	{
		sendClientMessage->message = "합성할 카드가 없습니다.";
	}
	else if(INFO_NEW_EVOLUTION == resultCompose)
	{
		sendClientMessage->message = "이제 각성 가능한 카드가 되었습니다.";
	}
	else
	{
		sendClientMessage->message = "합성 성공";
	}
	
	sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);

	return 0;
}

int CCommandController::cardAwake(shared_ptr<CLink> shared_clientInfo, const string & targetCardNum, MessageStruct * sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);
	int targetCardNumInt = stoi(targetCardNum);

	int resultAwake = mCardManager.AwakeCard(*clientInfo, targetCardNumInt);

	if (ERROR_NULL_CARD_ITERATOR == resultAwake)
	{
		sendClientMessage->message = "각성 실패.. 카드가 없습니다.";
	}
	else if (ERROR_COMPOSE_NO_EVOUTION_CARD == resultAwake)
	{
		sendClientMessage->message = "아직 각성 할 수 있는 카드가 아닙니다.";
	}
	else
	{
		sendClientMessage->message = "각성 완료";
	}
	sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
	return 0;
}

int CCommandController::cardSelect(shared_ptr<CLink> shared_clientInfo, MessageStruct* sendClientMessage)
{
	
	CLink* clientInfo = nullptr;
	char* resultCardName = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);

	if (!clientInfo->isMoneyOKGaChar())
	{
		sendClientMessage->message = "카드를 뽑기에 돈이 부족 합니다.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return CErrorHandler::ErrorHandler(ERROR_MONEY_FAIL);
	}

	int resultCardNum = mCardManager.GacharCard(*clientInfo, resultCardName);
	if (ERROR_GACHAR == resultCardNum)
	{
		sendClientMessage->message = "카드 뽑기 오류.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return CErrorHandler::ErrorHandler(ERROR_GACHAR);
	}

	sendClientMessage->message = resultCardName;
	sendClientMessage->sendRecvSize = strlen(resultCardName);
	return SUCCES_COMMAND_MESSAGE;
}


int CCommandController::commandHandling(shared_ptr<CLink> shared_clientInfo, vector<string>& commandString, MessageStruct* sendClientMessage)
{
	try
	{
		vector<string>::iterator iterBegin = commandString.begin();
		for (; iterBegin != commandString.end(); ++iterBegin)
		{
			cout << "명령 = " << (*iterBegin).c_str() << endl;
		}

		cout << "명령 처리 시작" << endl;
#pragma region 명령처리
		if (0 == commandString.at(0).compare(CommandEnter)) // 방에 입장
		{
			enterRoom(shared_clientInfo);
			sendClientMessage->message = "방에 입장 하셨습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandChannal))
		{
			changeChannel(shared_clientInfo);
			sendClientMessage->message = "채널을 변경 했습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandMakeRoom))
		{
			makeRoom(commandString.at(1).c_str(), shared_clientInfo);
			sendClientMessage->message = "방을 만들었습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandOutRoom))
		{
			outRoom(shared_clientInfo);
			sendClientMessage->message = "방에서 나왔습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandMergeRoom))
		{
			mergeRoom(shared_clientInfo);
			sendClientMessage->message = "방 병합 완료";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandChangeName))
		{
			CLink* clientInfo = nullptr;
			int channelNum = 0;
			readyCommand(shared_clientInfo, clientInfo, channelNum);
			// 기존 이름 변경
			clientInfo->changeName(commandString.at(1));
			cout << clientInfo->getMyName() << " 으로 이름 변경 됨" << endl;
			sendClientMessage->message = "이름 변경 되었습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandGachar))
		{
			return cardSelect(shared_clientInfo, sendClientMessage);
			//return SUCCES_COMMAND_MESSAGE;
		}
		else if (0 == commandString.at(0).compare(CommandCompose))
		{
			cardCompose(shared_clientInfo, commandString.at(1), commandString.at(2), sendClientMessage);
		}
		else if (0 == commandString.at(0).compare(CommandAwake))
		{
			cardAwake(shared_clientInfo, commandString.at(1), sendClientMessage);
		}
		else
		{
			sendClientMessage->message = "잘 못 된 명령 입니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
#pragma endregion
		return SUCCES_COMMAND;
	}
	catch (const std::exception&)
	{
		cout << "명령처리 오류" << endl;
		sendClientMessage->message = "잘 못 된 명령 입니다.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	}
	//if (commandString == nullptr)
	//	return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	
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