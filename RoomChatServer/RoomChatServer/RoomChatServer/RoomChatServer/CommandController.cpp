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
#pragma region ������ ��� ��
	RoomListIt roomBegin = RoomManager.getIterRoomBegin();
	RoomListIt roomEnd = RoomManager.getIterRoomEnd();
#pragma endregion
#pragma region ���� ���� �� �� �ִ� �� ã��
	for (; roomBegin != roomEnd; ++roomBegin)
	{
		CRoom* room = (*roomBegin).get();
		if (room->getChannelNum() == channelNum)
		{
			if (room->getAmountPeople() < EnterRoomPeopleLimit)
			{
				cout << "�濡 ����" << endl;
				if (!RoomHandler.enterRoom(shared_clientInfo, &RoomManager, room->getRoomNum()))
				{
					return CErrorHandler::ErrorHandler(ERROR_ENTER_ROOM);
				}
				// ä�ο����� ������
				if (!ChannelHandler.exitChannel(*clientInfo, ChannelManager))
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
	ChannelListIt channelBegin = ChannelManager.getIterChannelBegin(); // const iterator�� �ٲ�
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
			cout << moveChannelNum << "�� ä�� ����" << endl;
			break;
		}
	}
	return 0;
}

int CCommandController::makeRoom(char * command, shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);
	cout << "�� �����" << endl;
	char* roomName = RoomHandler.returnRoomName(command);
	if (!RoomHandler.makeRoom(shared_clientInfo, &RoomManager, roomName))
	{
		return CErrorHandler::ErrorHandler(ERROR_MAKE_ROOM);
	}
	//���� ä�ο����� ������
	if (!ChannelHandler.exitChannel(*clientInfo, ChannelManager))
		return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
	return 0;
}

int CCommandController::outRoom(shared_ptr<CLink> shared_clientInfo)
{
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);
	// �ٽ� ä�η� ���ư���
	if (!ChannelHandler.moveNextChannel(shared_clientInfo, ChannelManager, channelNum))
	{
		return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
	}
	cout << "�濡�� ������" << endl;
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
#pragma region Ǯ����� ��� �ʿ�?
	CRoom* myRoom = (*RoomManager.getMyRoomIter(channelNum, roomNum)).get();
	// Ǯ����� ��� �ʿ��Ѱ�? (�����ο� - ���� �� �ο�)
	int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->getAmountPeople());
#pragma endregion
	// ��ĥ ��� �� �˻�
	RoomListIt roomListBegin = RoomManager.getIterRoomBegin();
	RoomListIt roomListEnd = RoomManager.getIterRoomEnd();
	bool isMergeSucces = false;
	for (; roomListBegin != roomListEnd; ++roomListBegin)
	{
		if (roomNum == (*roomListBegin)->getRoomNum())
		{
			cout << "�� �ڽ� ��" << endl;
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
				RoomManager.eraseRoom(roomListBegin); // ��ĥ ��� �� ����Ʈ���� ����
			}
			cout << "�� ��ü �Ϸ�" << endl; isMergeSucces = true;
			break; // ���� ���� �˻��Ǵ� �ƹ� ��� ���� �� ��������
		}
	}
	if (!isMergeSucces)
		cout << "�� merge ����" << endl;
	return 0;
}

int CCommandController::cardSelect(shared_ptr<CLink> shared_clientInfo, MessageStruct* sendClientMessage)
{
	
	CLink* clientInfo = nullptr;
	int channelNum = 0;
	readyCommand(shared_clientInfo, clientInfo, channelNum);

	if (!clientInfo->isMoneyOKGaChar())
	{
		sendClientMessage->message = "ī�带 �̱⿡ ���� ���� �մϴ�.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return CErrorHandler::ErrorHandler(ERROR_MONEY_FAIL);
	}

	int randNum = mGacharHandler.randNumber();
	Card* gaCharResult = mGacharHandler.gaCharResult(randNum);
	if (nullptr == gaCharResult)
	{
		sendClientMessage->message = "ī�� �̱� ����.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
		return CErrorHandler::ErrorHandler(ERROR_GACHAR);
	}
	sendClientMessage->message = gaCharResult->name;
	sendClientMessage->sendRecvSize = strlen(gaCharResult->name);

	clientInfo->pushCard(gaCharResult);
	mCardManager.ChangeUserCardAmount(NameMemberCardInfoTxt.c_str(), clientInfo->getMyName(), gaCharResult->cardNum, true);
	return SUCCES_COMMAND_MESSAGE;
}


int CCommandController::commandHandling(shared_ptr<CLink> shared_clientInfo, char * command, MessageStruct* sendClientMessage)
{
	if (command == nullptr)
		return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	cout << "��� ó�� ����" << endl;
	command++;
#pragma region ���ó��
	if (*command == 'e') // �濡 ����
	{
		enterRoom(shared_clientInfo);
		sendClientMessage->message = "�濡 ���� �ϼ̽��ϴ�.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
	}
	else if (*command == 'c')
	{
		changeChannel(shared_clientInfo);
		sendClientMessage->message = "ä���� ���� �߽��ϴ�.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
	}
	else if (*command == 'm')
	{
		makeRoom(command, shared_clientInfo);
		sendClientMessage->message = "���� ��������ϴ�.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
	}
	else if (*command == 'o')
	{
		outRoom(shared_clientInfo);
		sendClientMessage->message = "�濡�� ���Խ��ϴ�.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
	}
	else if (*command == 'i')
	{
		mergeRoom(shared_clientInfo);
		sendClientMessage->message = "�� ���� �Ϸ�";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
	}
	else if (*command == 'n')
	{
		CLink* clientInfo = nullptr;
		int channelNum = 0;
		readyCommand(shared_clientInfo, clientInfo, channelNum);
		// ���� �̸� ����
		clientInfo->changeName(command, 1);
		cout << clientInfo->getMyName() << " ���� �̸� ���� ��" << endl;
		sendClientMessage->message = "�̸� ���� �Ǿ����ϴ�.";
		sendClientMessage->sendRecvSize = strlen(sendClientMessage->message);
	}
	else if (*command == 'g')
	{
		return cardSelect(shared_clientInfo, sendClientMessage);
		//return SUCCES_COMMAND_MESSAGE;
	}
#pragma endregion
	return SUCCES_COMMAND;
}

bool CCommandController::deleteClientSocket(CLink& clientInfo)
{
	cout << "���� ���� �̸� = " << clientInfo.getMyName() << endl;
	int myChannelNum = clientInfo.getMyChannelNum();
	int myRoomNum = clientInfo.getMyRoomNum();
	//�濡 �ֳ� ä�ο� �ֳ� Ȯ��
	if (NoneRoom == myRoomNum)
	{
		// ä���϶�
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
		// ���϶�
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