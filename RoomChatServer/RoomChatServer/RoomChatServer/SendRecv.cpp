#include "SendRecv.h"
#include "RoomChannelManager.h"


CSendRecv::CSendRecv()
{
}


CSendRecv::~CSendRecv()
{
}

int CSendRecv::sendn(CLink* clientInfo, CRoomChannelManager* roomChannelManager, int flags)
{
#pragma region �����޽���, ä��&�� ��ȣ ������
	MessageStruct* MS = clientInfo->getMessageStruct();
	if (MS->message == nullptr)
		return OccuredError;
	char* message = MS->message;
	int* size = MS->sendRecvSize;
	int channelNum = clientInfo->getMyChannelNum();
	int roomNum = clientInfo->getMyRoomNum();

	cout << "���� �޼��� = " << message << endl;
	cout << "���� ������ = " << *size << endl;
#pragma endregion
#pragma region ������ �� ���� �� ���� �ӽ� ����
	LinkListIt iterBegin;
	LinkListIt iterEnd;

	CChannel* myChannel;
	CRoom* myRoom;
#pragma endregion

#pragma region ���� ���� ä�� �� ��������
	if (roomNum == NoneRoom)
	{
		myChannel =  roomChannelManager->getMyChannel(channelNum);
		if (myChannel == nullptr)
		{
			cout << "���� ä�� ��ü�� ã�� �� �����ϴ�" << endl;
			return OccuredError;
		}
		// ä�� ���� ����Ʈ iter
		iterBegin = myChannel->getIterMyInfoBegin();
		iterEnd = myChannel->getIterMyInfoEnd();
	}
	else
	{
		CRoom* myRoom = *(roomChannelManager->getMyRoomIter(channelNum, roomNum));
		// �� ���� ����Ʈ iter
		if (myRoom == nullptr)
		{
			cout << "���� �水ü�� ã�� �� �����ϴ�" << endl;
			return OccuredError;
		}
		iterBegin = myRoom->getIterMyInfoBegin();
		iterEnd = myRoom->getIterMyInfoEnd();
	}
#pragma endregion
#pragma region ���� ���� ä�� or �濡�� ������
	// �濡 �ִ� ��� ������� ������
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		int temp = 0;
		// �޽��� ���� �ڽ��̸�
		if ((*iterBegin) == clientInfo)
		{
			continue; // ������ �ʰ� ���
		}
		SOCKET* clientSocket = (*iterBegin)->getClientSocket();
		send(*clientSocket, (char*)size, IntSize, flags); // ������ ������
		while (true)
		{
			temp += send(*clientSocket, message, *size, flags);
			if (temp >= *size)
				break;
		}
	}
#pragma endregion
	//cout << "�Ϸ�" << endl;
	return SuccesSend;
}

int CSendRecv::recvn(CLink* clientInfo, CCommandController* commandController, int flags)
{
#pragma region ���� ������ ũ�� ��������
	char* temp = new char[4];
	SOCKET* clientSocket = clientInfo->getClientSocket();
	MessageStruct* MS = clientInfo->getMessageStruct();
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
		return commandController->commandHandling(clientInfo, ptr);
	}
#pragma endregion
	cout << "���� �޽��� = " << MS->message << endl;
	return SuccesRecv;
}