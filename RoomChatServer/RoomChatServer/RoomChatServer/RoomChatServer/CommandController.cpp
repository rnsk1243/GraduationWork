#include "CommandController.h"
#include"ErrorHandler.h"
class CErrorHandler;

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
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
		// ���� �����
		int newRoomNumber = mRoomManager.MakeRoom(shared_clientInfo, roomName, battingMoney);
		// �뿡 ����
		EnterRoom(shared_clientInfo, newRoomNumber);
	}
}

void CCommandController::OutRoom(const LinkPtr & shared_clientInfo)
{
	 RoomListIt roomIter = mRoomManager.ExitRoom(shared_clientInfo);	// �뿡�� ������
	 if (true == (*roomIter)->IsRoomEmpty())			// �뿡 �ƹ��� ���� Ȯ��
	 {
		 mRoomManager.EraseRoom(roomIter);			// �ƹ��� ������ �� ����
	 }
}
void CCommandController::DeleteClientSocket(const LinkPtr & shared_clientInfo)
{

	int myChannelNum = shared_clientInfo.get()->GetMyChannelNum();
	int myRoomNum = shared_clientInfo.get()->GetMyRoomNum();
	//�濡 �ֳ� ä�ο� �ֳ� Ȯ��
	if (NoneRoom == myRoomNum)
	{
		// ä���϶�
		cout << "channel���� �����ϴ�." << endl;
		mChannelManager.ExitChannel(shared_clientInfo);
	}
	else
	{
		// ���϶�
		cout << "room���� �����ϴ�." << endl;
		OutRoom(shared_clientInfo);
	}
}

//bool CCommandController::MergeRoom(const shared_ptr<CLink>& shared_clientInfo, string& sendClientMessage)
//{
//	CLink* clientInfo = nullptr;
//	int channelNum = 0;
//	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
//	{
//		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
//		return false;
//	}
//	int roomNum = clientInfo->GetMyRoomNum();
//#pragma region Ǯ����� ��� �ʿ�?
//	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
//	// Ǯ����� ��� �ʿ��Ѱ�? (�����ο� - ���� �� �ο�)
//	int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->GetAmountPeople());
//#pragma endregion
//	// ��ĥ ��� �� �˻�
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
//			cout << "�� �ڽ� ��" << endl;
//			continue;
//		}
//		if ((*roomListBegin)->GetAmountPeople() <= limitToPeopleNum)
//		{
//			CRoom* targetRoom = (*roomListBegin).get();
//			if (myRoom->MergeRoom(targetRoom))
//			{
//				mRoomManager.EraseRoom(roomListBegin); // ��ĥ ��� �� ����Ʈ���� ����
//			}
//			cout << "�� ��ü �Ϸ�" << endl;
//			return true; // ���� ���� �˻��Ǵ� �ƹ� ��� ���� �� ��������
//		}
//	}
//	cout << "�� merge ����" << endl;
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
		shared_clientInfo.get()->SendnMine("ī�� ��ȭ ����");
	}
}

void CCommandController::SendAllReadyGameNotice(const LinkPtr & shared_clientInfo)
{
	if (mRoomManager.IsAllReadyGame(shared_clientInfo))
	{
		// ��޴����� ���� �� ��� �Լ� ȣ�� �� ��.
		mRoomManager.Broadcast(shared_clientInfo, "��� �÷��̾ �غ� �Ǿ����ϴ�. ī�带 ������.");
	}
	else
	{
		shared_clientInfo.get()->SendnMine("�ٸ� ��� �÷��̾ �غ� �Ǿ�� �մϴ�.");
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
		// ���� ���̰� ī��� �� ����. // �α� Ȯ���ؼ� ���� �ʿ�
		ErrorHandStatic->ErrorHandler(ERROR_GACHAR, shared_clientInfo);
	}
}


void CCommandController::CommandHandling(const LinkPtr& shared_clientInfo, vector<string>& commandString)
{
	try
	{
		if (0 == commandString.at(0).compare(CommandEnter)) // �濡 ����
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
			if (true == mChannelManager.EnterMyChannel(shared_clientInfo)) // ä�ο� ����
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
		cout << "���ó�� ����" << endl;
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, shared_clientInfo);
	}
}

