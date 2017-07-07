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

int CCommandController::ReadyCommand(shared_ptr<CLink> shared_clientInfo, CLink*& clientInfo, int& channelNum)
{
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
		channelNum = clientInfo->GetMyChannelNum();
	}
	else
	{
		return CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
	}
	return 0;
}

int CCommandController::EnterRoom(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr; 
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	bool isEnterSucces = false;
#pragma region 개설된 모든 방
	RoomListIt roomBegin = mRoomManager.GetIterRoomBegin();
	RoomListIt roomEnd = mRoomManager.GetIterRoomEnd();
#pragma endregion
#pragma region 내가 입장 할 수 있는 방 찾기
	for (; roomBegin != roomEnd; ++roomBegin)
	{
		CRoom* room = (*roomBegin).get();
		if (room->GetChannelNum() == channelNum)
		{
			if (room->GetAmountPeople() < EnterRoomPeopleLimit)
			{
				cout << "방에 입장" << endl;
				if (!mRoomHandler.EnterRoom(shared_clientInfo, &mRoomManager, room->GetRoomNum()))
				{
					return CErrorHandler::ErrorHandler(ERROR_ENTER_ROOM);
				}
				// 채널에서는 나가기
				if (!mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
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

int CCommandController::ChangeChannel(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	if (NoneRoom != clientInfo->GetMyRoomNum())
		return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	if (!mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
		return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
	if (channelNum == MaxChannelNum)
	{
		if (!mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, EnterChannelNum))
		{
			return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
		}
		return SUCCES_COMMAND;
	}
	ChannelListIt channelBegin = mChannelManager.GetIterChannelBegin(); // const iterator로 바꿈
	ChannelListIt channelEnd = mChannelManager.GetIterChannelEnd();
	for (; channelBegin != channelEnd; ++channelBegin)
	{
		if ((*channelBegin)->GetChannelNum() == channelNum)
		{
			++channelBegin;
			int moveChannelNum = (*channelBegin)->GetChannelNum();
			if (!mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, moveChannelNum))
			{
				return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
			}
			cout << moveChannelNum << "번 채널 변경" << endl;
			break;
		}
	}
	return 0;
}

int CCommandController::MakeRoom(const string& roomName, shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	cout << "방 만들기" << endl;
	if (!mRoomHandler.MakeRoom(shared_clientInfo, &mRoomManager, roomName))
	{
		return CErrorHandler::ErrorHandler(ERROR_MAKE_ROOM);
	}
	//원래 채널에서는 나가기
	if (!mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
		return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
	return 0;
}

int CCommandController::OutRoom(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	// 다시 채널로 돌아가고
	if (!mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, channelNum))
	{
		return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
	}
	cout << "방에서 나가기" << endl;
	if (!mRoomHandler.ExitRoom(clientInfo, &mRoomManager))
		return CErrorHandler::ErrorHandler(ERROR_EXIT_ROOM);
	return 0;
}

int CCommandController::MergeRoom(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);

	int roomNum = clientInfo->GetMyRoomNum();
#pragma region 풀방까지 몇명 필요?
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	// 풀방까지 몇명 필요한가? (제한인원 - 현재 방 인원)
	int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->GetAmountPeople());
#pragma endregion
	// 합칠 대상 방 검색
	RoomListIt roomListBegin = mRoomManager.GetIterRoomBegin();
	RoomListIt roomListEnd = mRoomManager.GetIterRoomEnd();
	bool isMergeSucces = false;
	for (; roomListBegin != roomListEnd; ++roomListBegin)
	{
		if (roomNum == (*roomListBegin)->GetRoomNum())
		{
			cout << "나 자신 방" << endl;
			continue;
		}
		if ((*roomListBegin)->GetAmountPeople() <= limitToPeopleNum)
		{
			if (0 >= (*roomListBegin).use_count())
			{
				return CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
			}
			CRoom* targetRoom = (*roomListBegin).get();
			if (myRoom->MergeRoom(targetRoom))
			{
				mRoomManager.EraseRoom(roomListBegin); // 합칠 대상 방 리스트에서 제거
			}
			cout << "방 합체 완료" << endl; isMergeSucces = true;
			break; // 가장 먼저 검색되는 아무 방과 병합 후 빠져나옴
		}
	}
	if (!isMergeSucces)
		cout << "방 merge 실패" << endl;
	return 0;
}

int CCommandController::CardCompose(shared_ptr<CLink> shared_clientInfo, const string& targetCardNum, const string& sourceCardNum, MessageStruct* sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);

	int targetCardNumInt = stoi(targetCardNum);
	int sourceCardNumInt = stoi(sourceCardNum);

	if (mCardManager.ComposeCard(*clientInfo, targetCardNumInt, sourceCardNumInt))
	{
		sendClientMessage->message = "합성 성공";
	}
	else
	{
		sendClientMessage->message = "합성 하는데 실패 하였습니다.";
	}	
	sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);

	return 0;
}

int CCommandController::CardEvolution(shared_ptr<CLink> shared_clientInfo, const string & targetCardNum, MessageStruct * sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	int targetCardNumInt = stoi(targetCardNum);

	if (mCardManager.EvolutionCard(*clientInfo, targetCardNumInt))
	{
		sendClientMessage->message = "진화 완료";
	}
	else
	{
		sendClientMessage->message = "진화 하는데 실패 하였습니다.";
	}
	sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
	return 0;
}

int CCommandController::CardSelect(shared_ptr<CLink> shared_clientInfo, MessageStruct* sendClientMessage)
{
	
	CLink* clientInfo = nullptr;
	char* resultCardName = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);

	if (!clientInfo->IsMoneyOKGaChar())
	{
		sendClientMessage->message = "카드를 뽑기에 돈이 부족 합니다.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return CErrorHandler::ErrorHandler(ERROR_MONEY_FAIL);
	}

	int resultCardNum = -1;
	
	if (false == mCardManager.GacharCard(*clientInfo, resultCardNum, resultCardName))
	{
		sendClientMessage->message = "카드 뽑기 오류.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return CErrorHandler::ErrorHandler(ERROR_GACHAR);
	}

	sendClientMessage->message = resultCardName;
	sendClientMessage->sendRecvSize = strlen(resultCardName);
	return SUCCES_COMMAND_MESSAGE;
}


int CCommandController::CommandHandling(shared_ptr<CLink> shared_clientInfo, vector<string>& commandString, MessageStruct* sendClientMessage)
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
			EnterRoom(shared_clientInfo);
			sendClientMessage->message = "방에 입장 하셨습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandChannal))
		{
			ChangeChannel(shared_clientInfo);
			sendClientMessage->message = "채널을 변경 했습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandMakeRoom))
		{
			MakeRoom(commandString.at(1).c_str(), shared_clientInfo);
			sendClientMessage->message = "방을 만들었습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandOutRoom))
		{
			OutRoom(shared_clientInfo);
			sendClientMessage->message = "방에서 나왔습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandMergeRoom))
		{
			MergeRoom(shared_clientInfo);
			sendClientMessage->message = "방 병합 완료";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandChangeName))
		{
			CLink* clientInfo = nullptr;
			int channelNum = 0;
			ReadyCommand(shared_clientInfo, clientInfo, channelNum);
			// 기존 이름 변경
			clientInfo->ChangeName(commandString.at(1));
			cout << clientInfo->GetMyName() << " 으로 이름 변경 됨" << endl;
			sendClientMessage->message = "이름 변경 되었습니다.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandGachar))
		{
			return CardSelect(shared_clientInfo, sendClientMessage);
			//return SUCCES_COMMAND_MESSAGE;
		}
		else if (0 == commandString.at(0).compare(CommandCompose))
		{
			CardCompose(shared_clientInfo, commandString.at(1), commandString.at(2), sendClientMessage);
		}
		else if (0 == commandString.at(0).compare(CommandEvolution))
		{
			CardEvolution(shared_clientInfo, commandString.at(1), sendClientMessage);
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

bool CCommandController::DeleteClientSocket(CLink& clientInfo)
{
	cout << "지울 소켓 이름 = " << clientInfo.GetMyName() << endl;
	int myChannelNum = clientInfo.GetMyChannelNum();
	int myRoomNum = clientInfo.GetMyRoomNum();
	//방에 있나 채널에 있나 확인
	if (NoneRoom == myRoomNum)
	{
		// 채널일때
		if (!mChannelHandler.ExitChannel(clientInfo, mChannelManager))
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
		if (!mRoomHandler.ExitRoom(&clientInfo, &mRoomManager))
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