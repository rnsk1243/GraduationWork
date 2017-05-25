#define _CRT_SECURE_NO_WARNINGS
#include "CommandController.h"
#include"ConstEnumInfo.h"

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
}


int CCommandController::commandHandling(CLink& clientInfo,const string& command, g_Message& sendServerMessage)
{
	if (&command == nullptr)
		return CErrorHandler::ErrorHandler(ERROR_COMMAND);
	cout << "��� ó�� ����" << endl;
	int channelNum = clientInfo.getMyChannelNum();
	int roomNum = clientInfo.getMyRoomNum();
#pragma region ���ó��
	if (0 == command.compare("EnterRoom")) // �濡 ����
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
					ChannelHandler.exitChannel(clientInfo, ChannelManager);
					isEnterSucces = true;
					string roomName = room->getRoomName();
					int amount = room->getAmountPeople();
					char amoutChar[10];
					_itoa(amount, amoutChar, 10);
					string roomAountPeople = amoutChar;
					if (EnterRoomPeopleLimit > amount)
					{
						sendServerMessage.set_message(roomName + " �� ���� ���� �ϼ̽��ϴ�. // ���� �� �ο� �� = " + roomAountPeople);
					}
					else if (EnterRoomPeopleLimit == amount)
					{
						sendServerMessage.set_message("Start");
						return SUCCES_RECV_EVERY_SEND;
					}
					
					
					break;
				}
			}
		}
#pragma endregion
		if (!isEnterSucces)
		{
			cout << "���� �� �� �ִ� ���� �����ϴ�." << endl;
			ChannelHandler.exitChannel(clientInfo, ChannelManager);
			cout << "�� �����" << endl;
			char roomName[RoomNameSize];
			const string roomNameStr = "room";
			strcpy_s(roomName, RoomNameSize, roomNameStr.c_str());
			cout << "������� �ϴ� �� �̸� = " << roomName << endl;
			RoomHandler.makeRoom(&clientInfo, &RoomManager, roomName);
			sendServerMessage.set_message("������ �� �ִ� ���� ��� " + roomNameStr + "���� �� ����� ���� �ϼ̽��ϴ�.");
		}
	}
	else if (0 == command.compare("NextChannel"))
	{
		if(NoneRoom != clientInfo.getMyRoomNum())
			return SUCCES_COMMAND;
		ChannelHandler.exitChannel(clientInfo, ChannelManager);
		if (channelNum == MaxChannelNum)
		{
			ChannelHandler.enterChannel(&clientInfo, ChannelManager, EnterChannelNum);
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
				ChannelHandler.enterChannel(&clientInfo, ChannelManager, moveChannelNum);
				cout << moveChannelNum << "�� ä�� ����" << endl;
				sendServerMessage.set_message(moveChannelNum + " ä�� ���� ���� �ϼ̽��ϴ�.");
				break;
			}
		}
		
	}
	else if (0 == command.compare("MakeRoom"))
	{
		//���� ä�ο����� ������
		ChannelHandler.exitChannel(clientInfo, ChannelManager);
		cout << "�� �����" << endl;
		char roomName[RoomNameSize];
		strcpy_s(roomName, RoomNameSize, command.c_str());
		cout << "������� �ϴ� �� �̸� = " << roomName << endl;
		RoomHandler.makeRoom(&clientInfo, &RoomManager, roomName);
		string name = roomName;
		sendServerMessage.set_message(name + "���� �� ����� ���� �ϼ̽��ϴ�.");
	}
	else if (0 == command.compare("OutRoom"))
	{
		// �ٽ� ä�η� ���ư���
		ChannelHandler.enterChannel(&clientInfo, ChannelManager, channelNum);
		cout << "�濡�� ������" << endl;
		RoomHandler.exitRoom(&clientInfo, &RoomManager);
		sendServerMessage.set_message("�濡�� ���Խ��ϴ�.");
	}
	else if (0 == command.compare("MergeRoom"))
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
				sendServerMessage.set_message("�� ��ü �Ϸ�");
				break; // ���� ���� �˻��Ǵ� �ƹ� ��� ���� �� ��������
			}
		}
		if (!isMergeSucces)
			cout << "�� merge ����" << endl;
	}
	//else if (*command == 'n')
	//{
	//	// ���� �̸� ����
	//	clientInfo.changeName(command, 1);
	//	cout << clientInfo.getMyName() << " ���� �̸� ���� ��" << endl;
	//}
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
		return ChannelHandler.exitChannel(clientInfo, ChannelManager);
	}
	else
	{
		// ���϶�
		return RoomHandler.exitRoom(&clientInfo, &RoomManager);
	}
	return false;
}