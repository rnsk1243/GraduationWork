#include "CommandController.h"
#include"ConstEnumInfo.h"
#include"ErrorHandler.h"

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
}


int CCommandController::commandHandling(CLink& clientInfo, char * command)
{
	if (command == nullptr)
		return CErrorHandler::ErrorHandler(ERROR_COMMAND);
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
					if (!RoomHandler.enterRoom(&clientInfo, &RoomManager, room->getRoomNum()))
					{
						return CErrorHandler::ErrorHandler(ERROR_ENTER_ROOM);
					}
					// ä�ο����� ������
					if(!ChannelHandler.exitChannel(clientInfo, ChannelManager))
						return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
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
			return CErrorHandler::ErrorHandler(ERROR_COMMAND);
		if (!ChannelHandler.exitChannel(clientInfo, ChannelManager))
			return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
		if (channelNum == MaxChannelNum)
		{
			if (!ChannelHandler.enterChannel(&clientInfo, ChannelManager, EnterChannelNum))
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
				if (!ChannelHandler.enterChannel(&clientInfo, ChannelManager, moveChannelNum))
				{
					return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
				}
				cout << moveChannelNum << "�� ä�� ����" << endl;
				break;
			}
		}
	}
	else if (*command == 'm')
	{
		//���� ä�ο����� ������
		if (!ChannelHandler.exitChannel(clientInfo, ChannelManager))
			return CErrorHandler::ErrorHandler(ERROR_EXIT_CHANNEL);
		cout << "�� �����" << endl;
		char* roomName = RoomHandler.returnRoomName(command);
		if (!RoomHandler.makeRoom(&clientInfo, &RoomManager, roomName))
		{
			return CErrorHandler::ErrorHandler(ERROR_MAKE_ROOM);
		}
	}
	else if (*command == 'o')
	{
		// �ٽ� ä�η� ���ư���
		if (!ChannelHandler.enterChannel(&clientInfo, ChannelManager, channelNum))
		{
			return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
		}
		cout << "�濡�� ������" << endl;
		if (!RoomHandler.exitRoom(&clientInfo, &RoomManager))
			return CErrorHandler::ErrorHandler(ERROR_EXIT_ROOM);

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
	return SUCCES_COMMAND;
}

bool CCommandController::deleteClientSocket(CLink& clientInfo)
{
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
		else { return true; }
	}
	else
	{
		// ���϶�
		if (!RoomHandler.exitRoom(&clientInfo, &RoomManager))
		{
			CErrorHandler::ErrorHandler(ERROR_EXIT_ROOM);
			return false;
		}
	}
	return false;
}