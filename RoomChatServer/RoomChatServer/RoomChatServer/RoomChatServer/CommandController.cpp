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
	cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
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
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
		return false;
	}
#pragma region ������ ��� ��
	RoomListIt roomBegin = mRoomManager.GetIterRoomBegin();
	RoomListIt roomEnd = mRoomManager.GetIterRoomEnd();
#pragma endregion
#pragma region ���� ���� �� �� �ִ� �� ã��
	for (; roomBegin != roomEnd; ++roomBegin)
	{
		CRoom* room = (*roomBegin).get();
		if (room->GetChannelNum() == channelNum)
		{
			if (room->GetAmountPeople() < EnterRoomPeopleLimit)
			{
				cout << "�濡 ����" << endl;
				if (false == mRoomHandler.EnterRoom(shared_clientInfo, &mRoomManager, room->GetRoomNum()))
				{
					ErrorHandStatic->ErrorHandler(ERROR_ENTER_ROOM, clientInfo);
					sendClientMessage = "�����ϴµ� ���� �Ͽ����ϴ�.";
					return false;
				}
				// ä�ο����� ������
				if (false == mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
				{
					ErrorHandStatic->ErrorHandler(ERROR_EXIT_CHANNEL, clientInfo);
					sendClientMessage = "ä�� ������ ���� �Ͽ����ϴ�.";
					return false;
				}
				sendClientMessage = "�濡 ���� �ϼ̽��ϴ�. �غ� �Ǽ����� /Start �� �Է��� �ּ���.";
				return true;
			}
		}
	}
#pragma endregion
	cout << "���� �� �� �ִ� ���� �����ϴ�." << endl;
	sendClientMessage = "���� �� �� �ִ� ���� �����ϴ�.";
	return false;
}

bool CCommandController::ChangeChannel(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
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
	ChannelListIt channelBegin = mChannelManager.GetIterChannelBegin(); // const iterator�� �ٲ�
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
			cout << moveChannelNum << "�� ä�� ����" << endl;
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
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
		return false;
	}
	cout << "�� �����" << endl;
	if (false == mRoomHandler.MakeRoom(shared_clientInfo, &mRoomManager, roomName, battingMoney))
	{
		ErrorHandStatic->ErrorHandler(ERROR_MAKE_ROOM, clientInfo);
		return false;
	}
	//���� ä�ο����� ������
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
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
		return false;
	}
	// �ٽ� ä�η� ���ư���
	if (false == mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, channelNum))
	{
		ErrorHandStatic->ErrorHandler(ERROR_ENTER_CHANNEL, clientInfo);
		return false;
	}
	cout << "�濡�� ������" << endl;
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
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
		return false;
	}
	int roomNum = clientInfo->GetMyRoomNum();
#pragma region Ǯ����� ��� �ʿ�?
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	// Ǯ����� ��� �ʿ��Ѱ�? (�����ο� - ���� �� �ο�)
	int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->GetAmountPeople());
#pragma endregion
	// ��ĥ ��� �� �˻�
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
			cout << "�� �ڽ� ��" << endl;
			continue;
		}
		if ((*roomListBegin)->GetAmountPeople() <= limitToPeopleNum)
		{
			CRoom* targetRoom = (*roomListBegin).get();
			if (myRoom->MergeRoom(targetRoom))
			{
				mRoomManager.EraseRoom(roomListBegin); // ��ĥ ��� �� ����Ʈ���� ����
			}
			cout << "�� ��ü �Ϸ�" << endl;
			return true; // ���� ���� �˻��Ǵ� �ƹ� ��� ���� �� ��������
		}
	}
	cout << "�� merge ����" << endl;
	return false;
}

bool CCommandController::CardCompose(shared_ptr<CLink> shared_clientInfo, const string& targetCardNum, const string& sourceCardNum, string& sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
		return false;
	}
	int targetCardNumInt = stoi(targetCardNum);
	int sourceCardNumInt = stoi(sourceCardNum);

	if (true == mCardManager.ComposeCard(*clientInfo, targetCardNumInt, sourceCardNumInt))
	{
		sendClientMessage = "�ռ� ����";
		return true;
	}
	else
	{
		sendClientMessage = "�ռ� �ϴµ� ���� �Ͽ����ϴ�.";
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
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
		return false;
	}
	int targetCardNumInt = stoi(targetCardNum);

	if (mCardManager.EvolutionCard(*clientInfo, targetCardNumInt))
	{
		sendClientMessage = "��ȭ �Ϸ�";
		return true;
	}
	else
	{
		sendClientMessage = "��ȭ �ϴµ� ���� �Ͽ����ϴ�.";
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
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
		return false;
	}
	clientInfo->SetReadyGame();
	int roomNum = clientInfo->GetMyRoomNum();
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	bool isAllReady = false;
	if (mRoomHandler.IsAllReadyGame(clientInfo, &mRoomManager))
	{
		sendClientMessage = "��� ������� ���� �غ� �Ǿ����ϴ�. ���� ī���ȣ�� �Է��� �ּ���.";
		isAllReady = true;
	}
	else
	{
		sendClientMessage = "����� �غ� �Ǽ����� ���� �غ� ���� ���� ���� ��ʴϴ�. Ȥ�� �濡 ��� ȥ�� �Դϴ�.";
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
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
		return false;
	}
	int roomNum = clientInfo->GetMyRoomNum();
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	if (true == myRoom->IsAllReady())
	{
		if (clientInfo->SetMyBattingCard(cardNum))
		{
			sendClientMessage = "ī�带 �������ϴ�!!! ����� ��ٸ�����.";
			return true;
		}
	}
	else
	{
		sendClientMessage = "���� ������ ��������?";
	}
	sendClientMessage = "ī��� ���̰� ���� ��� �ο��� ī�带 ���� �ʾҽ��ϴ�.";
	
	return false;
}

bool CCommandController::SendBattingResult(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage, SocketVec& clientSocks)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
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
			cout << "�¸��� = " << winnerPK;
			sendClientMessage = "��� ����";
			return true;
		}
		else
		{
			sendClientMessage = "����..";
			return false;
		}
	}
	sendClientMessage = "���� ��� ��� �غ� �ȵ�";

	return false;
}

bool CCommandController::CardSelect(shared_ptr<CLink> shared_clientInfo, string& sendClientMessage)
{
	
	CLink* clientInfo = nullptr;
	char* resultCardName = nullptr;
	int channelNum = 0;
	if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
	{
		sendClientMessage = "�� �� ���� ����..�˼��մϴ�.";
		return false;
	}
	if (!clientInfo->IsMoneyOKGaChar())
	{
		sendClientMessage = "ī�带 �̱⿡ ���� ���� �մϴ�.";
		ErrorHandStatic->ErrorHandler(ERROR_MONEY_FAIL, clientInfo);
		return false;
	}

	int resultCardNum = -1;
	
	if (false == mCardManager.GacharCard(*clientInfo, resultCardNum, resultCardName))
	{
		sendClientMessage = "ī�� �̱� ����.";
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
			cout << "��� = " << (*iterBegin).c_str() << endl;
		}

		cout << "��� ó�� ����" << endl;
#pragma region ���ó��
		if (0 == commandString.at(0).compare(CommandEnter)) // �濡 ����
		{
			EnterRoom(shared_clientInfo, sendClientMessage);
		}
		else if (0 == commandString.at(0).compare(CommandChannal))
		{
			ChangeChannel(shared_clientInfo, sendClientMessage);
			sendClientMessage = "ä���� ���� �߽��ϴ�.";
		}
		else if (0 == commandString.at(0).compare(CommandMakeRoom))
		{
			int battingMoney = stoi(commandString.at(2));
			MakeRoom(commandString.at(1).c_str(), shared_clientInfo, battingMoney, sendClientMessage);
			sendClientMessage = "���� ��������ϴ�.";
		}
		else if (0 == commandString.at(0).compare(CommandOutRoom))
		{
			OutRoom(shared_clientInfo, sendClientMessage);
			sendClientMessage = "�濡�� ���Խ��ϴ�.";
		}
		else if (0 == commandString.at(0).compare(CommandMergeRoom))
		{
			MergeRoom(shared_clientInfo, sendClientMessage);
			sendClientMessage = "�� ���� �Ϸ�";
		}
		else if (0 == commandString.at(0).compare(CommandChangeName))
		{
			CLink* clientInfo = nullptr;
			int channelNum = 0;
			if (false == ReadyCommand(shared_clientInfo, clientInfo, channelNum))
			{
				sendClientMessage = "�̸� ���� ����..";
				return false;
			}
			// ���� �̸� ����
			clientInfo->ChangeName(commandString.at(1));
			cout << clientInfo->GetMyName() << " ���� �̸� ���� ��" << endl;
			sendClientMessage = "�̸� ���� �Ǿ����ϴ�.";
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
			sendClientMessage = "�� �� �� ��� �Դϴ�.";
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
		cout << "���ó�� ����" << endl;
		sendClientMessage = "�� �� �� ��� �Դϴ�.";
		//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, clientInfo);
		return false;
	}
	//if (commandString == nullptr)
	//	return ErrorHandStatic->ErrorHandler(ERROR_COMMAND);
	
}

bool CCommandController::DeleteClientSocket(CLink& clientInfo)
{
	cout << "���� ���� �̸� = " << clientInfo.GetMyName() << endl;
	int myChannelNum = clientInfo.GetMyChannelNum();
	int myRoomNum = clientInfo.GetMyRoomNum();
	//�濡 �ֳ� ä�ο� �ֳ� Ȯ��
	if (NoneRoom == myRoomNum)
	{
		// ä���϶�
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
		// ���϶�
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