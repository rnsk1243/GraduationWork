#include "CommandController.h"
#include"ErrorHandler.h"
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
				if (!mRoomHandler.EnterRoom(shared_clientInfo, &mRoomManager, room->GetRoomNum()))
				{
					return CErrorHandler::ErrorHandler(ERROR_ENTER_ROOM);
				}
				// ä�ο����� ������
				if (!mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
					return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
				isEnterSucces = true;
				break;
			}
		}
	}
#pragma endregion
	if (!isEnterSucces)
		cout << "���� �� �� �ִ� ���� �����ϴ�." << endl;
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
				return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
			}
			cout << moveChannelNum << "�� ä�� ����" << endl;
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
	cout << "�� �����" << endl;
	if (!mRoomHandler.MakeRoom(shared_clientInfo, &mRoomManager, roomName))
	{
		return CErrorHandler::ErrorHandler(ERROR_MAKE_ROOM);
	}
	//���� ä�ο����� ������
	if (!mChannelHandler.ExitChannel(*clientInfo, mChannelManager))
		return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
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
		return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
	}
	cout << "�濡�� ������" << endl;
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
		if (roomNum == (*roomListBegin)->GetRoomNum())
		{
			cout << "�� �ڽ� ��" << endl;
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
		sendClientMessage->message = "��ȭ �Ϸ�";
	}
	else
	{
		sendClientMessage->message = "��ȭ �ϴµ� ���� �Ͽ����ϴ�.";
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
		sendClientMessage->message = "ī�带 �̱⿡ ���� ���� �մϴ�.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return CErrorHandler::ErrorHandler(ERROR_MONEY_FAIL);
	}

	int resultCardNum = -1;
	
	if (false == mCardManager.GacharCard(*clientInfo, resultCardNum, resultCardName))
	{
		sendClientMessage->message = "ī�� �̱� ����.";
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
			cout << "��� = " << (*iterBegin).c_str() << endl;
		}

		cout << "��� ó�� ����" << endl;
#pragma region ���ó��
		if (0 == commandString.at(0).compare(CommandEnter)) // �濡 ����
		{
			EnterRoom(shared_clientInfo);
			sendClientMessage->message = "�濡 ���� �ϼ̽��ϴ�.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandChannal))
		{
			ChangeChannel(shared_clientInfo);
			sendClientMessage->message = "ä���� ���� �߽��ϴ�.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandMakeRoom))
		{
			MakeRoom(commandString.at(1).c_str(), shared_clientInfo);
			sendClientMessage->message = "���� ��������ϴ�.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandOutRoom))
		{
			OutRoom(shared_clientInfo);
			sendClientMessage->message = "�濡�� ���Խ��ϴ�.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
		else if (0 == commandString.at(0).compare(CommandMergeRoom))
		{
			MergeRoom(shared_clientInfo);
			sendClientMessage->message = "�� ���� �Ϸ�";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
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
			sendClientMessage->message = "�� �� �� ��� �Դϴ�.";
			sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		}
#pragma endregion
		return SUCCES_COMMAND;
	}
	catch (const std::exception&)
	{
		cout << "���ó�� ����" << endl;
		sendClientMessage->message = "�� �� �� ��� �Դϴ�.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	}
	//if (commandString == nullptr)
	//	return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	
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
		// ���϶�
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