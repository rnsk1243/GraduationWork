#include "ActionNetWork.h"
#include "ChannelManager.h"
#include"RoomManager.h"
#include"ReadHandler.h"

CActionNetWork::CActionNetWork()
{
}


CActionNetWork::~CActionNetWork()
{
}

int CActionNetWork::recvn(SOCKET & socket, MessageStruct& MS, int flags)
{
	char temp[4];
	int isSuccess = recv(socket, temp, IntSize, flags);

	if (isSuccess == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return SOCKET_ERROR;
	}
	MS.sendRecvSize = *(int*)temp;

	// �ӽ÷� ���� temp �޸� ��ȯ
#pragma endregion
#pragma region �޼��� �ޱ�
	size_t left = MS.sendRecvSize;
	isSuccess = 0;
	while (left > 0)
	{
		isSuccess += recv(socket, MS.message, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	MS.message[left] = '\0';
#pragma endregion
	cout << "���� idPw�޽��� = " << MS.message << endl;
	return SuccesRecv;
}

int CActionNetWork::sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags)
{
#pragma region �����޽���, ä��&�� ��ȣ ������
	MessageStruct& MS = clientInfo.getMessageStruct();
	if (MS.message == nullptr)
		return OccuredError;
	char* message = MS.message;
	size_t size = MS.sendRecvSize;
	int channelNum = clientInfo.getMyChannelNum();
	int roomNum = clientInfo.getMyRoomNum();

	cout << "���� �޼��� = " << message << endl;
	cout << "���� ������ = " << size << endl;
#pragma endregion
#pragma region ������ �� ���� �� ���� �ӽ� ����
	LinkListIt iterBegin;
	LinkListIt iterEnd;

	CChannel* myChannel = nullptr;
	CRoom* myRoom = nullptr;
#pragma endregion

#pragma region ���� ���� ä�� �� ��������
	if (roomNum == NoneRoom)
	{
		myChannel =  channelManager.getMyChannel(channelNum);
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
		CRoom* myRoom = *(roomManager.getMyRoomIter(channelNum, roomNum));
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
		size_t temp = 0;
		// �޽��� ���� �ڽ��̸�
		if ((*iterBegin) == &clientInfo)
		{
			continue; // ������ �ʰ� ���
		}

		SOCKET& clientSocket = (*iterBegin)->getClientSocket();

		sendMyName(clientSocket, clientInfo); // �̸� ������

		send(clientSocket, (char*)&size, IntSize, flags); // ������ ������
		while (true)
		{
			temp += send(clientSocket, message, size, flags);
			if (temp >= size)
				break;
		}
	}
#pragma endregion
	//cout << "�Ϸ�" << endl;
	return SuccesSend;
}

int CActionNetWork::sendn(SOCKET & socket, MessageStruct & MS, int flags)
{
	char* message = MS.message;
	size_t size = MS.sendRecvSize;
	send(socket, (char*)&size, IntSize, flags); // ������ ������
	size_t temp = 0;
	while (true)
	{
		temp += send(socket, message, size, flags);
		if (temp >= size)
			break;
	}
	return SuccesSend;
}

int CActionNetWork::recvn(CLink& clientInfo, CCommandController& commandController, int flags)
{
#pragma region ���� ������ ũ�� ��������
	char temp[4];
	SOCKET& clientSocket = clientInfo.getClientSocket();
	MessageStruct& MS = clientInfo.getMessageStruct();
	int isSuccess = recv(clientSocket, temp, IntSize, flags);

	if (isSuccess == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return SOCKET_ERROR;
	}
	MS.sendRecvSize = *(int*)temp;
#pragma endregion
#pragma region �޼��� �ޱ�
	size_t left = MS.sendRecvSize;
	isSuccess = 0;
	while (left > 0)
	{
		isSuccess += recv(clientSocket, MS.message, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	MS.message[left] = '\0';
#pragma endregion
#pragma region ��ɸ޽��� �̸� ó��
	char* ptr = strchr(MS.message, '/');
	if (ptr != nullptr)
	{
		return commandController.commandHandling(clientInfo, ptr);
	}
#pragma endregion
	cout << "���� �޽��� = " << MS.message << endl;
	return SuccesRecv;
}

int CActionNetWork::sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags)
{
	if (nullptr == clientInfo.getMyName())
	{
		cout << "�̸� ����" << endl;
		return NullNameError;
	}
	MessageStruct& myName = clientInfo.getMyNameMessageStruct();
	myName.message = clientInfo.getMyName();
	myName.sendRecvSize = strlen(myName.message);

	cout << "���� �޼��� = " << myName.message << endl;
	cout << "���� ������ = " << myName.sendRecvSize << endl;

	int temp = 0;
	temp = send(clientSocket, (char*)&myName.sendRecvSize, IntSize, flags); // ������ ������
	if (IntSize == temp)
	{
		temp = 0;
		while (true)
		{
			temp += send(clientSocket, myName.message, myName.sendRecvSize, flags);
			if (temp >= myName.sendRecvSize)
				break;
		}
	}
	return myName.sendRecvSize;
}
