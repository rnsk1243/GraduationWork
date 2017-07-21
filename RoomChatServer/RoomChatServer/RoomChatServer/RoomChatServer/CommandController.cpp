#include "CommandController.h"
#include"ErrorHandler.h"
#include"ActionNetWork.h"
class CErrorHandler;
//#include"ConstEnumInfo.h"
//#include"GaChar.h"

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
}

bool CCommandController::ReadyCommand(shared_ptr<CLink> shared_clientInfo, CLink*& clientInfo, int& channelNum)
{
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
		channelNum = clientInfo->GetMyChannelNum();
		return true;
	}
	else
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
		return false;
	}
	return false;
}

bool CCommandController::EnterRoom(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage)
{
	CLink* clientInfo = nullptr; 
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
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
				if (false == mRoomHandler.EnterRoom(shared_clientInfo, &mRoomManager, room->GetRoomNum()))
				{
					ErrorHandStatic->ErrorHandler(ERROR_ENTER_ROOM, clientInfo);
					sendClientMessage = "입장하는데 실패 하였습니다.";
					return false;
				}
				// 채널에서는 나가기
				if (false == mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
				{
					ErrorHandStatic->ErrorHandler(ERROR_EXIT_CHANNEL, clientInfo);
					sendClientMessage = "채널 나가기 실패 하였습니다.";
					return false;
				}
				sendClientMessage = "방에 입장 하셨습니다. 준비 되셨으면 /Start 를 입력해 주세요.";
				return true;
			}
		}
	}
#pragma endregion
	cout << "입장 할 수 있는 방이 없습니다." << endl;
	sendClientMessage = "입장 할 수 있는 방이 없습니다.";
	return false;
}

bool CCommandController::ChangeChannel(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	if (NoneRoom != clientInfo->GetMyRoomNum())
	{
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, clientInfo);
		return false;
	}
	if (!mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
	{
		ErrorHandStatic->ErrorHandler(ERROR_EXIT_CHANNEL, clientInfo);
		return false;
	}
	if (channelNum == MaxChannelNum)
	{
		if (false == mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, EnterChannelNum))
		{
			ErrorHandStatic->ErrorHandler(ERROR_ENTER_CHANNEL, clientInfo);
			return false;
		}
		//SUCCES_COMMAND;
		return true;
	}
	ChannelListIt channelBegin = mChannelManager.GetIterChannelBegin(); // const iterator로 바꿈
	ChannelListIt channelEnd = mChannelManager.GetIterChannelEnd();
	for (; channelBegin != channelEnd; ++channelBegin)
	{
		if ((*channelBegin)->GetChannelNum() == channelNum)
		{
			++channelBegin;
			int moveChannelNum = (*channelBegin)->GetChannelNum();
			if (false == mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, moveChannelNum))
			{
				ErrorHandStatic->ErrorHandler(ERROR_ENTER_CHANNEL, clientInfo);
				return true;
			}
			cout << moveChannelNum << "번 채널 변경" << endl;
			break;
		}
	}
	return false;
}

bool CCommandController::MakeRoom(const string& roomName, shared_ptr<CLink> shared_clientInfo,const int& battingMoney, string& sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	cout << "방 만들기" << endl;
	if (false == mRoomHandler.MakeRoom(shared_clientInfo, &mRoomManager, roomName, battingMoney))
	{
		ErrorHandStatic->ErrorHandler(ERROR_MAKE_ROOM, clientInfo);
		return false;
	}
	//원래 채널에서는 나가기
	if (false == mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
	{
		ErrorHandStatic->ErrorHandler(ERROR_EXIT_CHANNEL, clientInfo);
		return false;
	}
	return false;
}

bool CCommandController::OutRoom(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	// 다시 채널로 돌아가고
	if (false == mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, channelNum))
	{
		ErrorHandStatic->ErrorHandler(ERROR_ENTER_CHANNEL, clientInfo);
		return false;
	}
	cout << "방에서 나가기" << endl;
	if (false == mRoomHandler.ExitRoom(clientInfo, &mRoomManager))
	{
		ErrorHandStatic->ErrorHandler(ERROR_EXIT_ROOM, clientInfo);
		return false;
	}
	return true;
}

bool CCommandController::MergeRoom(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	int roomNum = clientInfo->GetMyRoomNum();
#pragma region 풀방까지 몇명 필요?
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	// 풀방까지 몇명 필요한가? (제한인원 - 현재 방 인원)
	int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->GetAmountPeople());
#pragma endregion
	// 합칠 대상 방 검색
	RoomListIt roomListBegin = mRoomManager.GetIterRoomBegin();
	RoomListIt roomListEnd = mRoomManager.GetIterRoomEnd();

	for (; roomListBegin != roomListEnd; ++roomListBegin)
	{
		if (0 >= (*roomListBegin).use_count())
		{
			ErrorHandStatic->ErrorHandler(ERROR_SHARED_ROOM_COUNT_ZORO, clientInfo);
			return false;
		}
		if (roomNum == (*roomListBegin)->GetRoomNum())
		{
			cout << "나 자신 방" << endl;
			continue;
		}
		if ((*roomListBegin)->GetAmountPeople() <= limitToPeopleNum)
		{
			CRoom* targetRoom = (*roomListBegin).get();
			if (myRoom->MergeRoom(targetRoom))
			{
				mRoomManager.EraseRoom(roomListBegin); // 합칠 대상 방 리스트에서 제거
			}
			cout << "방 합체 완료" << endl;
			return true; // 가장 먼저 검색되는 아무 방과 병합 후 빠져나옴
		}
	}
	cout << "방 merge 실패" << endl;
	return false;
}

bool CCommandController::CardCompose(shared_ptr<CLink> shared_clientInfo, const string& targetCardNum, const string& sourceCardNum, string& sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	int targetCardNumInt = stoi(targetCardNum);
	int sourceCardNumInt = stoi(sourceCardNum);

	if (true == mCardManager.ComposeCard(*clientInfo, targetCardNumInt, sourceCardNumInt))
	{
		sendClientMessage = "합성 성공";
		return true;
	}
	else
	{
		sendClientMessage = "합성 하는데 실패 하였습니다.";
		return false;
	}	
	return false;
}

bool CCommandController::CardEvolution(shared_ptr<CLink> shared_clientInfo, const string & targetCardNum, string& sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	int targetCardNumInt = stoi(targetCardNum);

	if (mCardManager.EvolutionCard(*clientInfo, targetCardNumInt))
	{
		sendClientMessage = "진화 완료";
		return true;
	}
	else
	{
		sendClientMessage = "진화 하는데 실패 하였습니다.";
		return false;
	}
	return false;
}

bool CCommandController::SendAllReadyGameNotice(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage, SocketVec& clientSocks)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	clientInfo->SetReadyGame();
	int roomNum = clientInfo->GetMyRoomNum();
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	bool isAllReady = false;
	if (mRoomHandler.IsAllReadyGame(clientInfo, &mRoomManager))
	{
		sendClientMessage = "모든 사람들이 게임 준비가 되었습니다. 내실 카드번호를 입력해 주세요.";
		isAllReady = true;
	}
	else
	{
		sendClientMessage = "당신은 준비가 되셨지만 아직 준비가 되지 않은 분이 계십니다. 혹은 방에 당신 혼자 입니다.";
		isAllReady = false;
	}
	
	if (false == myRoom->GetRoomSockets(clientSocks))
	{
		return false;
	}
	return isAllReady;
}

bool CCommandController::IsHaveCard(shared_ptr<CLink> shared_clientInfo, int cardNum, string& sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	int roomNum = clientInfo->GetMyRoomNum();
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	if (true == myRoom->IsAllReady())
	{
		if (clientInfo->SetMyBattingCard(cardNum))
		{
			sendClientMessage = "카드를 던졌습니다!!! 결과를 기다리세요.";
			return true;
		}
	}
	else
	{
		sendClientMessage = "정말 가지고 있으세요?";
	}
	sendClientMessage = "카드는 내셨고 아직 모든 인원이 카드를 내지 않았습니다.";
	
	return false;
}

bool CCommandController::SendBattingResult(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage, SocketVec& clientSocks)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	int roomNum = clientInfo->GetMyRoomNum();
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	if (false == myRoom->GetRoomSockets(clientSocks))
	{
		return false;
	}
	if (true == myRoom->IsAllReadyBatting())
	{
		int winnerPK = myRoom->BattingResult();
		if (-1 != winnerPK)
		{
			cout << "승리자 = " << winnerPK;
			sendClientMessage = "결과 나옴";
			return true;
		}
		else
		{
			sendClientMessage = "에러..";
			return false;
		}
	}
	sendClientMessage = "아직 모든 사람 준비 안됨";

	return false;
}

bool CCommandController::CardSelect(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage)
{
	
	CLink* clientInfo = nullptr;
	char* resultCardName = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "알 수 없는 에러..죄송합니다.";
		return false;
	}
	if (!clientInfo->IsMoneyOKGaChar())
	{
		sendClientMessage = "카드를 뽑기에 돈이 부족 합니다.";
		ErrorHandStatic->ErrorHandler(ERROR_MONEY_FAIL, clientInfo);
		return false;
	}

	int resultCardNum = -1;
	
	if (false == mCardManager.GacharCard(*clientInfo, resultCardNum, resultCardName))
	{
		sendClientMessage = "카드 뽑기 오류.";
		ErrorHandStatic->ErrorHandler(ERROR_GACHAR, clientInfo);
		return false;
	}

	sendClientMessage = resultCardName;
	ErrorHandStatic->ErrorHandler(SUCCES_COMMAND_MESSAGE, clientInfo);
	return true;
}


bool CCommandController::CommandHandling(shared_ptr<CLink> shared_clientInfo, vector<string>& commandString, string& sendClientMessage, SocketVec& clientSocks)
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
			EnterRoom(shared_clientInfo, sendClientMessage);
		}
		else if (0 == commandString.at(0).compare(CommandChannal))
		{
			ChangeChannel(shared_clientInfo, sendClientMessage);
			sendClientMessage = "채널을 변경 했습니다.";
		}
		else if (0 == commandString.at(0).compare(CommandMakeRoom))
		{
			int battingMoney = stoi(commandString.at(2));
			MakeRoom(commandString.at(1).c_str(), shared_clientInfo, battingMoney, sendClientMessage);
			sendClientMessage = "방을 만들었습니다.";
		}
		else if (0 == commandString.at(0).compare(CommandOutRoom))
		{
			OutRoom(shared_clientInfo, sendClientMessage);
			sendClientMessage = "방에서 나왔습니다.";
		}
		else if (0 == commandString.at(0).compare(CommandMergeRoom))
		{
			MergeRoom(shared_clientInfo, sendClientMessage);
			sendClientMessage = "방 병합 완료";
		}
		else if (0 == commandString.at(0).compare(CommandChangeName))
		{
			CLink* clientInfo = nullptr;
			int channelNum = 0;
			if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
			{
				sendClientMessage = "이름 변경 실패..";
				return false;
			}
			// 기존 이름 변경
			clientInfo->ChangeName(commandString.at(1));
			cout << clientInfo->GetMyName() << " 으로 이름 변경 됨" << endl;
			sendClientMessage = "이름 변경 되었습니다.";
		}
		else if (0 == commandString.at(0).compare(CommandGachar))
		{
			CardSelect(shared_clientInfo, sendClientMessage);
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
		else if (0 == commandString.at(0).compare(CommandGameStart))
		{
			SendAllReadyGameNotice(shared_clientInfo, sendClientMessage, clientSocks);
		}
		else if (0 == commandString.at(0).compare(CommandGameCardSubmit))
		{
			int cardNum = stoi(commandString.at(1));
			if (IsHaveCard(shared_clientInfo, cardNum, sendClientMessage))
			{
				SendBattingResult(shared_clientInfo, sendClientMessage, clientSocks);
			}
		}
		else
		{
			sendClientMessage = "잘 못 된 명령 입니다.";
			//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
#pragma endregion
		return true;
	}
	catch (const std::exception&)
	{
		CLink* clientInfo = nullptr;
		int channelNum = 0;
		ReadyCommand(shared_clientInfo, clientInfo, channelNum);
		cout << "명령처리 오류" << endl;
		sendClientMessage = "잘 못 된 명령 입니다.";
		//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, clientInfo);
		return false;
	}
	//if (commandString == nullptr)
	//	return ErrorHandStatic->ErrorHandler(ERROR_COMMAND);
	
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
			ErrorHandStatic->ErrorHandler(ERROR_EXIT_CHANNEL, &clientInfo);
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
			ErrorHandStatic->ErrorHandler(ERROR_EXIT_ROOM, &clientInfo);
			return false;
		}
		else 
		{
			return true;
		}
	}
	return false;
}