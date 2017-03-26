#include "CommandController.h"



CCommandController::CCommandController(CRoomChannelManager* roomChannelManager, CChannelHandler* channelHandler, CRoomHandler* roomHandler)
	:RoomChannelManager(roomChannelManager), 
	ChannelHandler(channelHandler), 
	RoomHandlere(roomHandler)
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
}


int CCommandController::commandHandling(CLink* clientInfo, char * command)
{
	if (command == nullptr)
		return OccuredError;
	cout << "��� ó�� ����" << endl;
	int channelNum = clientInfo->getMyChannelNum();
	int roomNum = clientInfo->getMyRoomNum();
	*command++;
#pragma region ���ó��
	if (*command == 'e') // �濡 ����
	{
		bool isEnterSucces = false;
#pragma region ������ ��� ��
		RoomListIt roomBegin = RoomChannelManager->getIterRoomBegin();
		RoomListIt roomEnd = RoomChannelManager->getIterRoomEnd();
#pragma endregion
#pragma region ���� ���� �� �� �ִ� �� ã��
		for (; roomBegin != roomEnd; ++roomBegin)
		{
			CRoom* room = *roomBegin;
			if (room->getChannelNum() == channelNum)
			{
				if (room->getAmountPeople() < EnterRoomPeopleLimit)
				{
					cout << "�濡 ����" << endl;
					RoomHandlere->enterRoom(clientInfo, RoomChannelManager, room->getRoomNum());
					// ä�ο����� ������
					ChannelHandler->exitChannel(clientInfo, RoomChannelManager);
					isEnterSucces = true;
				}
			}
		}
#pragma endregion
		if(!isEnterSucces)
			cout << "���� �� �� �ִ� ���� �����ϴ�." << endl;
	}
	else if (*command == 'c')
	{
		if(NoneRoom != clientInfo->getMyRoomNum())
			return SuccesCommand;
		ChannelHandler->exitChannel(clientInfo, RoomChannelManager);
		if (channelNum == MaxChannelNum)
		{
			ChannelHandler->enterChannel(clientInfo, RoomChannelManager, 0);
			return SuccesCommand;
		}
		ChannelListIt channelBegin = RoomChannelManager->getIterChannelBegin();
		ChannelListIt channelEnd = RoomChannelManager->getIterChannelEnd();
		for (; channelBegin != channelEnd; ++channelBegin)
		{
			if ((*channelBegin)->getChannelNum() == channelNum)
			{
				++channelBegin;
				int moveChannelNum = (*channelBegin)->getChannelNum();
				ChannelHandler->enterChannel(clientInfo, RoomChannelManager, moveChannelNum);
				cout << moveChannelNum << "�� ä�� ����" << endl;
			}
		}
	}
	else if (*command == 'm')
	{
		//���� ä�ο����� ������
		ChannelHandler->exitChannel(clientInfo, RoomChannelManager);
		cout << "�� �����" << endl;
		char* roomName = RoomHandlere->returnRoomName(command);
		RoomHandlere->makeRoom(clientInfo, RoomChannelManager, roomName);
	}
	else if (*command == 'o')
	{
		// �ٽ� ä�η� ���ư���
		ChannelHandler->enterChannel(clientInfo, RoomChannelManager, channelNum);
		cout << "�濡�� ������" << endl;
		RoomHandlere->exitRoom(clientInfo, RoomChannelManager);

	}
	else if (*command == 'i')
	{
#pragma region Ǯ����� ��� �ʿ�?
		CRoom* myRoom = *RoomChannelManager->getMyRoomIter(channelNum, roomNum);
		// Ǯ����� ��� �ʿ��Ѱ�? (�����ο� - ���� �� �ο�)
		int limitToPeopleNum = EnterRoomPeopleLimit - (myRoom->getAmountPeople());
#pragma endregion
		// ��ĥ ��� �� �˻�
		RoomListIt roomListBegin = RoomChannelManager->getIterRoomBegin();
		RoomListIt roomListEnd = RoomChannelManager->getIterRoomEnd();
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
				CRoom* targetRoom = (*roomListBegin);
				if (myRoom->mergeRoom(targetRoom))
				{
					RoomChannelManager->eraseRoom(roomListBegin); // ��ĥ ��� �� ����Ʈ���� ����
					delete targetRoom; // �� ����
				}
				cout << "�� ��ü �Ϸ�" << endl; isMergeSucces = true;
				break; // ���� ���� �˻��Ǵ� �ƹ� ��� ���� �� ��������
			}
		}
		if (!isMergeSucces)
			cout << "�� merge ����" << endl;
	}
	else if (*command == 'n')
	{
		// ���� �̸� ����
		clientInfo->changeName(command, 1);
		cout << clientInfo->getMyName() << " ���� �̸� ���� ��" << endl;
	}
#pragma endregion
	return SuccesCommand;
}

bool CCommandController::deleteClientSocket(CLink* clientInfo)
{
	int myChannelNum = clientInfo->getMyChannelNum();
	int myRoomNum = clientInfo->getMyRoomNum();
	//�濡 �ֳ� ä�ο� �ֳ� Ȯ��
	if (NoneRoom == myRoomNum)
	{
		// ä���϶�
		return ChannelHandler->exitChannel(clientInfo, RoomChannelManager);
	}
	else
	{
		// ���϶�
		return RoomHandlere->exitRoom(clientInfo, RoomChannelManager);
	}
	return false;
}