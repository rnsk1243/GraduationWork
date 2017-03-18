#include "NetWork.h"



CNetWork::CNetWork(CLink* link, CMyInfo* myInfo, CChannelHandlere* channelHandlere, CRoomHandlere* roomHandlere)
	:Link(link), ClientInfo(myInfo), ChannelHandlere(channelHandlere), RoomHandlere(roomHandlere)
{
}


CNetWork::~CNetWork()
{
	cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
	deleteClientSocket();// client������ �켱 ��orä�� ����Ʈ���� ����� ����
	delete ClientInfo;
}

int CNetWork::sendn(int flags)
{
#pragma region �����޽���, ä��&�� ��ȣ ������
	MessageStruct* MS = ClientInfo->getMessageStruct();
	if (MS->message == nullptr)
		return OccuredError;
	char* message = MS->message;
	int* size = MS->sendRecvSize;
	int channelNum = ClientInfo->getMyChannelNum();
	int roomNum = ClientInfo->getMyRoomNum();

	cout << "���� �޼��� = " << message << endl;
	cout << "���� ������ = " << *size << endl;
#pragma endregion
#pragma region ������ �� ���� �� ���� �ӽ� ����
	SocketListIt iterBegin;
	SocketListIt iterEnd;

	CChannel* myChannel;
	CRoom* myRoom;
#pragma endregion

#pragma region ���� ���� ä�� �� ��������
	if (roomNum == NoneRoom)
	{
		myChannel = Link->getMyChannel(channelNum);
		if (myChannel == nullptr)
		{
			cout << "���� ä�� ��ü�� ã�� �� �����ϴ�" << endl;
			return OccuredError;
		}
		// ä�� ���� ����Ʈ iter
		iterBegin = myChannel->getIterBegin();
		iterEnd = myChannel->getIterEnd();
	}
	else
	{
		CRoom* myRoom = *(Link->getMyRoomIter(channelNum, roomNum));
		// �� ���� ����Ʈ iter
		if (myRoom == nullptr)
		{
			cout << "���� �水ü�� ã�� �� �����ϴ�" << endl;
			return OccuredError;
		}
		iterBegin = myRoom->getIterSocketBegin();
		iterEnd = myRoom->getIterSocketEnd();
	}
#pragma endregion
#pragma region ���� ���� ä�� or �濡�� ������
	// �濡 �ִ� ��� ������� ������
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		int temp = 0;
		// �޽��� ���� �ڽ��̸�
		if ((*iterBegin) == ClientInfo->getClientSocket())
		{
			continue; // ������ �ʰ� ���
		}
		send(*(*iterBegin), (char*)size, IntSize, flags); // ������ ������
		while (true)
		{
			temp += send(*(*iterBegin), message, *size, flags);
			if (temp >= *size)
				break;
		}
	}
#pragma endregion
	//cout << "�Ϸ�" << endl;
	return SuccesSend;
}

int CNetWork::recvn(int flags)
{
#pragma region ���� ������ ũ�� ��������
	char* temp = new char[4];
	SOCKET* clientSocket = ClientInfo->getClientSocket();
	MessageStruct* MS = ClientInfo->getMessageStruct();
	int isSuccess = recv(*clientSocket, temp, IntSize, flags);

	if (isSuccess == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return SOCKET_ERROR;
	}
	*MS->sendRecvSize = *(int*)temp;

	// �ӽ÷� ���� temp �޸� ��ȯ
	delete temp;
#pragma endregion
#pragma region �޼��� �ޱ�
	int left = *MS->sendRecvSize;
	while (left > 0)
	{
		isSuccess += recv(*clientSocket, MS->message, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	MS->message[left] = '\0';
#pragma endregion
#pragma region ��ɸ޽��� �̸� ó��
	char* ptr = strchr(MS->message, '/');
	if (ptr != nullptr)
	{
		return commandHandling(ptr);
	}
#pragma endregion
	cout << "���� �޽��� = " << MS->message << endl;
	return SuccesRecv;
}

bool CNetWork::deleteClientSocket()
{
	int myChannelNum = ClientInfo->getMyChannelNum();
	int myRoomNum = ClientInfo->getMyRoomNum();
	//�濡 �ֳ� ä�ο� �ֳ� Ȯ��
	if (NoneRoom == myRoomNum)
	{
		// ä���϶�
		CChannel* myChannel = Link->getMyChannel(myChannelNum);
		SocketListIt socketBegin = myChannel->getIterBegin();
		SocketListIt socketEnd = myChannel->getIterEnd();
		for (; socketBegin != socketEnd; ++socketBegin)
		{
			if ((*socketBegin) == ClientInfo->getClientSocket())
			{
				myChannel->eraseSocket(socketBegin);
				return true;
			}
		}
	}
	else 
	{
		// ���϶�
		RoomListIt myRoomIter = Link->getMyRoomIter(myChannelNum, myRoomNum);
		CRoom* myRoom = (*myRoomIter);
		SocketListIt socketBegin = myRoom->getIterSocketBegin();
		SocketListIt socketEnd = myRoom->getIterSocketEnd();
		for (; socketBegin != socketEnd; ++socketBegin)
		{
			if ((*socketBegin) == ClientInfo->getClientSocket())
			{
				ClientInfo->setMyRoomNum(NoneRoom); // �濡 ������ ����
				myRoom->eraseSocket(socketBegin);
				return true;
			}
		}
	}
	return false;
}

int CNetWork::commandHandling(char * command)
{
	if (command == nullptr)
		return OccuredError;
	cout << "��� ó�� ����" << endl;
	int channelNum = ClientInfo->getMyChannelNum();
	int roomNum = ClientInfo->getMyRoomNum();
	*command++;
#pragma region ���ó��
	if (*command == 'e') // �濡 ����
	{
		bool isEnterSucces = false;
#pragma region ������ ��� ��
		RoomListIt roomBegin = Link->getIterRoomBegin();
		RoomListIt roomEnd = Link->getIterRoomEnd();
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
					RoomHandlere->enterRoom(ClientInfo, Link, room->getRoomNum());
					// ä�ο����� ������
					ChannelHandlere->exitChannel(ClientInfo, Link);
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
		if(NoneRoom != ClientInfo->getMyRoomNum())
			return SuccesCommand;
		ChannelHandlere->exitChannel(ClientInfo, Link);
		if (channelNum == MaxChannelNum)
		{
			ChannelHandlere->enterChannel(ClientInfo, Link, 0);
			return SuccesCommand;
		}
		ChannelListIt channelBegin = Link->getIterChannelBegin();
		ChannelListIt channelEnd = Link->getIterChannelEnd();
		for (; channelBegin != channelEnd; ++channelBegin)
		{
			if ((*channelBegin)->getChannelNum() == channelNum)
			{
				++channelBegin;
				int moveChannelNum = (*channelBegin)->getChannelNum();
				ChannelHandlere->enterChannel(ClientInfo, Link, moveChannelNum);
				cout << moveChannelNum << "�� ä�� ����" << endl;
			}
		}
	}
	else if (*command == 'm')
	{
		//���� ä�ο����� ������
		ChannelHandlere->exitChannel(ClientInfo, Link);
		cout << "�� �����" << endl;
		char* roomName = RoomHandlere->returnRoomName(command);
		RoomHandlere->makeRoom(ClientInfo, Link, roomName);
	}
	else if (*command == 'o')
	{
		// �ٽ� ä�η� ���ư���
		ChannelHandlere->enterChannel(ClientInfo, Link, channelNum);
		cout << "�濡�� ������" << endl;
		RoomHandlere->exitRoom(ClientInfo, Link);

	}
#pragma endregion
	return SuccesCommand;
}
