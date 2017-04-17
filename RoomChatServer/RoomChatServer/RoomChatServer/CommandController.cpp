#include "CommandController.h"



CCommandController::CCommandController(CChannelManager& channelManager, CRoomManager& roomManager, CChannelHandler& channelHandler, CRoomHandler& roomHandler)
	:ChannelManager(channelManager),
	RoomManager(roomManager), 
	ChannelHandler(channelHandler), 
	RoomHandler(roomHandler)
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
}


int CCommandController::commandHandling(CLink& clientInfo, char * command)
{
	if (command == nullptr)
		return OccuredError;
	cout << "��� ó�� ����" << endl;
	int channelNum = clientInfo.getMyChannelNum();
	int roomNum = clientInfo.getMyRoomNum();
	command++;
#pragma region ���ó��
	if (*command == 'e') // �濡 ����
	{
		bool isEnterSucces = false;
#pragma region ������ ��� ��
		RoomListIt roomBegin = RoomManager.getIterRoomBegin();
		RoomListIt roomEnd = RoomManager.getIterRoomEnd();
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
					RoomHandler.enterRoom(&clientInfo, &RoomManager, room->getRoomNum());
					// ä�ο����� ������
					ChannelHandler.exitChannel(&clientInfo, &ChannelManager);
					isEnterSucces = true;
					break;
				}
			}
		}
#pragma endregion
		if(!isEnterSucces)
			cout << "���� �� �� �ִ� ���� �����ϴ�." << endl;
	}
	else if (*command == 'c')
	{
		if(NoneRoom != clientInfo.getMyRoomNum())
			return SuccesCommand;
		ChannelHandler.exitChannel(&clientInfo, &ChannelManager);
		if (channelNum == MaxChannelNum)
		{
			ChannelHandler.enterChannel(&clientInfo, &ChannelManager, EnterChannelNum);
			return SuccesCommand;
		}
		ChannelListIt channelBegin = ChannelManager.getIterChannelBegin(); // const iterator�� �ٲ�
		ChannelListIt channelEnd = ChannelManager.getIterChannelEnd();
		for (; channelBegin != channelEnd; ++channelBegin)
		{
			if ((*channelBegin)->getChannelNum() == channelNum)
			{
				++channelBegin;
				int moveChannelNum = (*channelBegin)->getChannelNum();
				ChannelHandler.enterChannel(&clientInfo, &ChannelManager, moveChannelNum);
				cout << moveChannelNum << "�� ä�� ����" << endl;
				break;
			}
		}
	}
	else if (*command == 'm')
	{
		//���� ä�ο����� ������
		ChannelHandler.exitChannel(&clientInfo, &ChannelManager);
		cout << "�� �����" << endl;
		char* roomName = RoomHandler.returnRoomName(command);
		RoomHandler.makeRoom(&clientInfo, &RoomManager, roomName);
	}
	else if (*command == 'o')
	{
		// �ٽ� ä�η� ���ư���
		ChannelHandler.enterChannel(&clientInfo, &ChannelManager, channelNum);
		cout << "�濡�� ������" << endl;
		RoomHandler.exitRoom(&clientInfo, &RoomManager);

	}
	else if (*command == 'i')
	{
#pragma region Ǯ����� ��� �ʿ�?
		CRoom* myRoom = *RoomManager.getMyRoomIter(channelNum, roomNum);
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
				CRoom* targetRoom = (*roomListBegin);
				if (myRoom->mergeRoom(targetRoom))
				{
					RoomManager.eraseRoom(roomListBegin); // ��ĥ ��� �� ����Ʈ���� ����
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
		clientInfo.changeName(command, 1);
		cout << clientInfo.getMyName() << " ���� �̸� ���� ��" << endl;
	}
#pragma endregion
	return SuccesCommand;
}

bool CCommandController::deleteClientSocket(CLink& clientInfo)
{
	int myChannelNum = clientInfo.getMyChannelNum();
	int myRoomNum = clientInfo.getMyRoomNum();
	//�濡 �ֳ� ä�ο� �ֳ� Ȯ��
	if (NoneRoom == myRoomNum)
	{
		// ä���϶�
		return ChannelHandler.exitChannel(&clientInfo, &ChannelManager);
	}
	else
	{
		// ���϶�
		return RoomHandler.exitRoom(&clientInfo, &RoomManager);
	}
	return false;
}