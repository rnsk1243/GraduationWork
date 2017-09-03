#include "CommandController.h"
#include"ErrorHandler.h"
class CErrorHandler;

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
}

void CCommandController::SetEnterChannel(const LinkPtr & shared_clientInfo, const int & moveChannelNumber)
{
	if (nullptr != shared_clientInfo.get())
	{
		if (ChannelAmount >= moveChannelNumber && StartEnterChannelNum <= moveChannelNumber)
		{
			if (0 == shared_clientInfo.get()->GetMyChannelNum())
			{
				mChannelManager.MoveChannel(shared_clientInfo, moveChannelNumber);
			}
		}
	}
}

void CCommandController::EnterRoom(const LinkPtr & shared_clientInfo, const int& roomNumber)
{
	if (mRoomManager.EnterRoom(shared_clientInfo, roomNumber))
	{
		mChannelManager.ExitChannel(shared_clientInfo);
	}
}

void CCommandController::ChangeChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber)
{
	if (nullptr != shared_clientInfo.get())
	{
		if (false == shared_clientInfo.get()->IsRoomEnterState())
		{
			if (ChannelAmount >= moveChannelNumber && StartEnterChannelNum <= moveChannelNumber)
			{
				if (mChannelManager.ExitChannel(shared_clientInfo))
				{
					mChannelManager.MoveChannel(shared_clientInfo, moveChannelNumber);
				}
			}
		}	
	}
}

void CCommandController::MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName, const int & battingMoney)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr != client && (false == client->IsRoomEnterState()))
	{
		// 룸을 만들고
		int newRoomNumber = mRoomManager.MakeRoom(shared_clientInfo, roomName, battingMoney);
		// 룸에 들어가고
		EnterRoom(shared_clientInfo, newRoomNumber);
	}
}

void CCommandController::OutRoom(const LinkPtr & shared_clientInfo)
{
	 RoomListIt roomIter = mRoomManager.ExitRoom(shared_clientInfo);	// 룸에서 나가기
	 if (true == (*roomIter)->IsRoomEmpty())			// 룸에 아무도 없나 확인
	 {
		 mRoomManager.EraseRoom(roomIter);			// 아무도 없으면 룸 삭제
	 }
}
void CCommandController::DeleteClientSocket(const LinkPtr & shared_clientInfo)
{

	int myChannelNum = shared_clientInfo.get()->GetMyChannelNum();
	int myRoomNum = shared_clientInfo.get()->GetMyRoomNum();
	//방에 있나 채널에 있나 확인
	if (NoneRoom == myRoomNum)
	{
		// 채널일때
		cout << "channel에서 나갑니다." << endl;
		mChannelManager.ExitChannel(shared_clientInfo);
	}
	else
	{
		// 방일때
		cout << "room에서 나갑니다." << endl;
		OutRoom(shared_clientInfo);
	}
}

//bool CCommandController::MergeRoom(const shared_ptr<CLink>& shared_clientInfo, string& sendClientMessage)
//{
//	CLink* clientInfo = nullptr;
//	int channelNum = 0;
//	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
//	{
//		sendClientMessage = "알 수 없는 에러..죄송합니다.";
//		return false;
//	}
//	int roomNum = clientInfo->GetMyRoomNum();
//#pragma region 풀방까지 몇명 필요?
//	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
//	// 풀방까지 몇명 필요한가? (제한인원 - 현재 방 인원)
//	int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->GetAmountPeople());
//#pragma endregion
//	// 합칠 대상 방 검색
//	RoomListIt roomListBegin = mRoomManager.GetIterRoomBegin();
//	RoomListIt roomListEnd = mRoomManager.GetIterRoomEnd();
//
//	for (; roomListBegin != roomListEnd; ++roomListBegin)
//	{
//		if (0 >= (*roomListBegin).use_count())
//		{
//			ErrorHandStatic->ErrorHandler(ERROR_SHARED_ROOM_COUNT_ZORO, clientInfo);
//			return false;
//		}
//		if (roomNum == (*roomListBegin)->GetRoomNum())
//		{
//			cout << "나 자신 방" << endl;
//			continue;
//		}
//		if ((*roomListBegin)->GetAmountPeople() <= limitToPeopleNum)
//		{
//			CRoom* targetRoom = (*roomListBegin).get();
//			if (myRoom->MergeRoom(targetRoom))
//			{
//				mRoomManager.EraseRoom(roomListBegin); // 합칠 대상 방 리스트에서 제거
//			}
//			cout << "방 합체 완료" << endl;
//			return true; // 가장 먼저 검색되는 아무 방과 병합 후 빠져나옴
//		}
//	}
//	cout << "방 merge 실패" << endl;
//	return false;
//}

void CCommandController::CardCompose(const LinkPtr& shared_clientInfo, const string& targetCardNum, const string& sourceCardNum)
{
	int targetCardNumInt = stoi(targetCardNum);
	int sourceCardNumInt = stoi(sourceCardNum);
	mCardManager.ComposeCard(shared_clientInfo, targetCardNumInt, sourceCardNumInt);
}

void CCommandController::CardEvolution(const LinkPtr& shared_clientInfo, const string & targetCardNum)
{
	int targetCardNumInt = stoi(targetCardNum);
	if (mCardManager.EvolutionCard(shared_clientInfo, targetCardNumInt))
	{
		shared_clientInfo.get()->SendnMine("카드 진화 성공");
	}
}

void CCommandController::SendAllReadyGameNotice(const LinkPtr & shared_clientInfo)
{
	if (mRoomManager.IsAllReadyGame(shared_clientInfo))
	{
		// 룸메니저를 통해 방 멤버 함수 호출 할 것.
		mRoomManager.Broadcast(shared_clientInfo, "모든 플레이어가 준비 되었습니다. 카드를 내세요.");
	}
	else
	{
		shared_clientInfo.get()->SendnMine("다른 모든 플레이어가 준비 되어야 합니다.");
	}
	
}

void CCommandController::SetBattingCard(const LinkPtr& shared_clientInfo, const int& cardNum)
{
	if (true == shared_clientInfo.get()->GetReadyGame())
	{
		shared_clientInfo.get()->SetMyBattingCard(cardNum);
	}
}

void CCommandController::SendBattingResult(const LinkPtr& shared_clientInfo)
{
	mRoomManager.ResultBatting(shared_clientInfo);
}

CCommandController * CCommandController::GetInstance()
{
	if (nullptr == CommandControllerStatic)
	{
		CommandControllerStatic = new CCommandController();
	}
	return CommandControllerStatic;
}

void CCommandController::CardSelect(const LinkPtr& shared_clientInfo)
{
	if (false == mCardManager.GacharCard(shared_clientInfo))
	{
		// 돈만 깍이고 카드는 못 뽑음. // 로그 확인해서 보상 필요
		ErrorHandStatic->ErrorHandler(ERROR_GACHAR, shared_clientInfo);
	}
}


void CCommandController::CommandHandling(const LinkPtr& shared_clientInfo, vector<string>& commandString)
{
	try
	{
		if (0 == commandString.at(0).compare(CommandEnter)) // 방에 입장
		{
			EnterRoom(shared_clientInfo, stoi(commandString.at(1)));
		}
		else if (0 == commandString.at(0).compare(CommandChannal))
		{
			ChangeChannel(shared_clientInfo, stoi(commandString.at(1)));
		}
		else if (0 == commandString.at(0).compare(CommandMakeRoom))
		{
			int battingMoney = stoi(commandString.at(2));
			MakeRoom(shared_clientInfo, commandString.at(1), battingMoney);
		}
		else if (0 == commandString.at(0).compare(CommandOutRoom))
		{
			if (true == mChannelManager.EnterMyChannel(shared_clientInfo)) // 채널에 들어가기
			{
				OutRoom(shared_clientInfo);
			}
		}
		else if (0 == commandString.at(0).compare(CommandGachar))
		{
			CardSelect(shared_clientInfo);
		}
		else if (0 == commandString.at(0).compare(CommandCompose))
		{
			CardCompose(shared_clientInfo, commandString.at(1), commandString.at(2));
		}
		else if (0 == commandString.at(0).compare(CommandEvolution))
		{
			CardEvolution(shared_clientInfo, commandString.at(1));
		}
		else if (0 == commandString.at(0).compare(CommandGameStart))
		{
			SendAllReadyGameNotice(shared_clientInfo);
		}
		else if (0 == commandString.at(0).compare(CommandGameCardSubmit))
		{
			SetBattingCard(shared_clientInfo, stoi(commandString.at(1)));
			SendBattingResult(shared_clientInfo);
		}
		else
		{
			if (shared_clientInfo.get()->IsRoomEnterState())
			{
				mRoomManager.Talk(shared_clientInfo, commandString.at(0));
			}
			else
			{
				mChannelManager.Talk(shared_clientInfo, commandString.at(0));
			}
		}
	}
	catch (const std::exception&)
	{
		int channelNum = 0;
		cout << "명령처리 오류" << endl;
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, shared_clientInfo);
	}
}

