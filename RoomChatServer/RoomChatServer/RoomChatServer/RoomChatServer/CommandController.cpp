#include "CommandController.h"
#include"ErrorHandler.h"
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

int CCommandController::ReadyCommand(shared_ptr<CLink> shared_clientInfo, CLink*& clientInfo, int& channelNum)
{
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
		channelNum = clientInfo->GetMyChannelNum();
	}
	else
	{
		return ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
	}
	return 0;
}

bool CCommandController::EnterRoom(shared_ptr<CLink> shared_clientInfo, MessageStruct* sendClientMessage)
{
	CLink* clientInfo = nullptr; 
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
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
					sendClientMessage->message = "�����ϴµ� ���� �Ͽ����ϴ�.";
					return false;
				}
				// ä�ο����� ������
				if (false == mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
				{
					ErrorHandStatic->ErrorHandler(ERROR_EXIT_CHANNEL, clientInfo);
					sendClientMessage->message = "ä�� ������ ���� �Ͽ����ϴ�.";
					return false;
				}
				sendClientMessage->message = "�濡 ���� �ϼ̽��ϴ�. �غ� �Ǽ����� /Start �� �Է��� �ּ���.";
				return true;
			}
		}
	}
#pragma endregion
	cout << "���� �� �� �ִ� ���� �����ϴ�." << endl;
	sendClientMessage->message = "���� �� �� �ִ� ���� �����ϴ�.";
	return false;
}

int CCommandController::ChangeChannel(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	if (NoneRoom != clientInfo->GetMyRoomNum())
		return ErrorHandStatic->ErrorHandler(ERROR_COMMAND, clientInfo);
	if (!mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
		return ErrorHandStatic->ErrorHandler(ERROR_EXIT_CHANNEL, clientInfo);
	if (channelNum == MaxChannelNum)
	{
		if (!mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, EnterChannelNum))
		{
			return ErrorHandStatic->ErrorHandler(ERROR_ENTER_CHANNEL, clientInfo);
		}
		return SUCCES_COMMAND;
	}
	ChannelListIt channelBegin = mChannelManager.GetIterChannelBegin(); // const iterator�� �ٲ�
	ChannelListIt channelEnd = mChannelManager.GetIterChannelEnd();
	for (; channelBegin != channelEnd; ++channelBegin)
	{
		if ((*channelBegin)->GetChannelNum() == channelNum)
		{
			++channelBegin;
			int moveChannelNum = (*channelBegin)->GetChannelNum();
			if (!mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, moveChannelNum))
			{
				return ErrorHandStatic->ErrorHandler(ERROR_ENTER_CHANNEL, clientInfo);
			}
			cout << moveChannelNum << "�� ä�� ����" << endl;
			break;
		}
	}
	return 0;
}

int CCommandController::MakeRoom(const string& roomName, shared_ptr<CLink> shared_clientInfo,const int& battingMoney)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	cout << "�� �����" << endl;
	if (!mRoomHandler.MakeRoom(shared_clientInfo, &mRoomManager, roomName, battingMoney))
	{
		return ErrorHandStatic->ErrorHandler(ERROR_MAKE_ROOM, clientInfo);
	}
	//���� ä�ο����� ������
	if (!mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
		return ErrorHandStatic->ErrorHandler(ERROR_EXIT_CHANNEL, clientInfo);
	return 0;
}

int CCommandController::OutRoom(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	// �ٽ� ä�η� ���ư���
	if (!mChannelHandler.MoveNextChannel(shared_clientInfo, mChannelManager, channelNum))
	{
		return ErrorHandStatic->ErrorHandler(ERROR_ENTER_CHANNEL, clientInfo);
	}
	cout << "�濡�� ������" << endl;
	if (!mRoomHandler.ExitRoom(clientInfo, &mRoomManager))
		return ErrorHandStatic->ErrorHandler(ERROR_EXIT_ROOM, clientInfo);
	return 0;
}

int CCommandController::MergeRoom(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);

	int roomNum = clientInfo->GetMyRoomNum();
#pragma region Ǯ����� ��� �ʿ�?
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	// Ǯ����� ��� �ʿ��Ѱ�? (�����ο� - ���� �� �ο�)
	int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->GetAmountPeople());
#pragma endregion
	// ��ĥ ��� �� �˻�
	RoomListIt roomListBegin = mRoomManager.GetIterRoomBegin();
	RoomListIt roomListEnd = mRoomManager.GetIterRoomEnd();
	bool isMergeSucces = false;
	for (; roomListBegin != roomListEnd; ++roomListBegin)
	{
		if (0 >= (*roomListBegin).use_count())
		{
			return ErrorHandStatic->ErrorHandler(ERROR_SHARED_ROOM_COUNT_ZORO, clientInfo);
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
			cout << "�� ��ü �Ϸ�" << endl; isMergeSucces = true;
			break; // ���� ���� �˻��Ǵ� �ƹ� ��� ���� �� ��������
		}
	}
	if (!isMergeSucces)
		cout << "�� merge ����" << endl;
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
		sendClientMessage->message = "�ռ� ����";
	}
	else
	{
		sendClientMessage->message = "�ռ� �ϴµ� ���� �Ͽ����ϴ�.";
	}	
	//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);

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
		sendClientMessage->message = "��ȭ �Ϸ�";
	}
	else
	{
		sendClientMessage->message = "��ȭ �ϴµ� ���� �Ͽ����ϴ�.";
	}
	//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
	return 0;
}

bool CCommandController::SendAllReadyGameNotice(shared_ptr<CLink> shared_clientInfo, MessageStruct * sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	clientInfo->SetReadyGame();
	if (mRoomHandler.IsAllReadyGame(clientInfo, &mRoomManager))
	{
		sendClientMessage->message = "��� ������� ���� �غ� �Ǿ����ϴ�. ���� ī���ȣ�� �Է��� �ּ���.";
		return true;
	}
	sendClientMessage->message = "����� �غ� �Ǽ����� ���� �غ� ���� ���� ���� ��ʴϴ�. Ȥ�� �濡 ��� ȥ�� �Դϴ�.";
	return false;
}

bool CCommandController::IsHaveCard(shared_ptr<CLink> shared_clientInfo, int cardNum, MessageStruct* sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);

	int roomNum = clientInfo->GetMyRoomNum();
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	if (true == myRoom->IsAllReady())
	{
		if (clientInfo->SetMyBattingCard(cardNum))
		{
			sendClientMessage->message = "ī�带 �������ϴ�!!! ����� ��ٸ�����.";
			return true;
		}
		sendClientMessage->message = "���� ������ ��������?";
	}
	sendClientMessage->message = "ī��� ���̰� ���� ��� �ο��� ī�带 ���� �ʾҽ��ϴ�.";
	
	return false;
}

bool CCommandController::SendBattingResult(shared_ptr<CLink> shared_clientInfo, MessageStruct * sendClientMessage)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);
	int roomNum = clientInfo->GetMyRoomNum();
	CRoom* myRoom = (*mRoomManager.GetMyRoomIter(channelNum, roomNum)).get();
	if (true == myRoom->IsAllReadyBatting())
	{
		CLink* winner = myRoom->BattingResult();
		cout << "�¸��� = " << winner->GetMyName();
		sendClientMessage->message = "��� ����";
		return true;
	}
	sendClientMessage->message = "���� ��� ��� �غ� �ȵ�";
	return false;
}

int CCommandController::CardSelect(shared_ptr<CLink> shared_clientInfo, MessageStruct* sendClientMessage)
{
	
	CLink* clientInfo = nullptr;
	char* resultCardName = nullptr;
	int channelNum = 0;
	ReadyCommand(shared_clientInfo, clientInfo, channelNum);

	if (!clientInfo->IsMoneyOKGaChar())
	{
		sendClientMessage->message = "ī�带 �̱⿡ ���� ���� �մϴ�.";
		//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return ErrorHandStatic->ErrorHandler(ERROR_MONEY_FAIL, clientInfo);
	}

	int resultCardNum = -1;
	
	if (false == mCardManager.GacharCard(*clientInfo, resultCardNum, resultCardName))
	{
		sendClientMessage->message = "ī�� �̱� ����.";
		//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return ErrorHandStatic->ErrorHandler(ERROR_GACHAR, clientInfo);
	}

	sendClientMessage->message = resultCardName;
	//sendClientMessage->sendRecvSize = strlen(resultCardName);
	return SUCCES_COMMAND_MESSAGE;
}


int CCommandController::CommandHandling(shared_ptr<CLink> shared_clientInfo, vector<string>& commandString, MessageStruct* sendClientMessage)
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
			//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandChannal))
		{
			ChangeChannel(shared_clientInfo);
			sendClientMessage->message = "ä���� ���� �߽��ϴ�.";
			//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandMakeRoom))
		{
			int battingMoney = stoi(commandString.at(2));
			MakeRoom(commandString.at(1).c_str(), shared_clientInfo, battingMoney);
			sendClientMessage->message = "���� ��������ϴ�.";
			//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandOutRoom))
		{
			OutRoom(shared_clientInfo);
			sendClientMessage->message = "�濡�� ���Խ��ϴ�.";
			//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandMergeRoom))
		{
			MergeRoom(shared_clientInfo);
			sendClientMessage->message = "�� ���� �Ϸ�";
			//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandChangeName))
		{
			CLink* clientInfo = nullptr;
			int channelNum = 0;
			ReadyCommand(shared_clientInfo, clientInfo, channelNum);
			// ���� �̸� ����
			clientInfo->ChangeName(commandString.at(1));
			cout << clientInfo->GetMyName() << " ���� �̸� ���� ��" << endl;
			sendClientMessage->message = "�̸� ���� �Ǿ����ϴ�.";
			//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
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
			SendAllReadyGameNotice(shared_clientInfo, sendClientMessage);
		}
		else if (0 == commandString.at(0).compare(CommandGameCardSubmit))
		{
			int cardNum = stoi(commandString.at(1));
			if (IsHaveCard(shared_clientInfo, cardNum, sendClientMessage))
			{
				SendBattingResult(shared_clientInfo, sendClientMessage);
			}
		}
		else
		{
			sendClientMessage->message = "�� �� �� ��� �Դϴ�.";
			//sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
#pragma endregion
		return SUCCES_COMMAND;
	}
	catch (const std::exception&)
	{
		CLink* clientInfo = nullptr;
		int channelNum = 0;
		ReadyCommand(shared_clientInfo, clientInfo, channelNum);
		cout << "���ó�� ����" << endl;
		sendClientMessage->message = "�� �� �� ��� �Դϴ�.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return ErrorHandStatic->ErrorHandler(ERROR_COMMAND, clientInfo);
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