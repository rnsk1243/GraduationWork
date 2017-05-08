#define _CRT_SECURE_NO_WARNINGS
#include "ActionNetWork.h"
#include "ChannelManager.h"
#include"RoomManager.h"
#include"ReadHandler.h"
#include"ErrorHandler.h"


CActionNetWork::CActionNetWork()
{
}


CActionNetWork::~CActionNetWork()
{
}

int CActionNetWork::recvn(SOCKET & socket, MessageStruct& MS, int flags)
{
	char temp[4];
	size_t isSuccess = recv(socket, temp, IntSize, flags);

	if (isSuccess == SOCKET_ERROR)
	{
		return CErrorHandler::ErrorHandler(ERROR_RECV);
	}
	MS.sendRecvSize = *(int*)temp;

	// �ӽ÷� ���� temp �޸� ��ȯ
#pragma endregion
#pragma region �޼��� �ޱ�
	size_t left = MS.sendRecvSize;
	isSuccess = 0;
	while (left > 0)
	{
		isSuccess += recv(socket, MS.message, (int)left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			return CErrorHandler::ErrorHandler(ERROR_RECV);
		}
		else if (isSuccess >= left)
			break;
	}
	MS.message[left] = '\0';
#pragma endregion
	cout << "���� idPw�޽��� = " << MS.message << endl;
	return SUCCES_RECV;
}

int CActionNetWork::sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags)
{
#pragma region �����޽���, ä��&�� ��ȣ ������
	MessageStruct& MS = clientInfo.getMessageStruct();
	char* message = MS.message;
	size_t size = MS.sendRecvSize;
	int channelNum = clientInfo.getMyChannelNum();
	int roomNum = clientInfo.getMyRoomNum();

	//cout << "���� �޼��� = " << message << endl;
	//cout << "���� ������ = " << size << endl;
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
			return CErrorHandler::ErrorHandler(ERROR_GET_CHANNEL);
		}
		// ä�� ���� ����Ʈ iter
		iterBegin = myChannel->getIterMyInfoBegin();
		iterEnd = myChannel->getIterMyInfoEnd();
	}
	else
	{
		CRoom* myRoom = (*(roomManager.getMyRoomIter(channelNum, roomNum))).get();
		// �� ���� ����Ʈ iter
		if (myRoom == nullptr)
		{
			cout << "���� �水ü�� ã�� �� �����ϴ�" << endl;
			return CErrorHandler::ErrorHandler(ERROR_GET_ROOM);
		}
		iterBegin = myRoom->getIterMyInfoBegin();
		iterEnd = myRoom->getIterMyInfoEnd();
	}
#pragma endregion
#pragma region ���� ���� ä�� or �濡�� ������
	int isSuccess = 0;
	// �濡 �ִ� ��� ������� ������
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		size_t temp = 0;
		// �޽��� ���� �ڽ��̸�
		if ((*iterBegin).get() == &clientInfo)
		{
			continue; // ������ �ʰ� ���
		}

		SOCKET& clientSocket = (*iterBegin)->getClientSocket();

		sendMyName(clientSocket, clientInfo); // �̸� ������

		isSuccess = send(clientSocket, (char*)&size, IntSize, flags); // ������ ������
		if (isSuccess == SOCKET_ERROR)
			return CErrorHandler::ErrorHandler(ERROR_SEND);
		while (true)
		{
			temp += send(clientSocket, message, (int)size, flags);
			if (temp == SOCKET_ERROR)
				return CErrorHandler::ErrorHandler(ERROR_SEND);
			if (temp >= size)
				break;
		}
	}
#pragma endregion
	//cout << "�Ϸ�" << endl;
	return SUCCES_SEND;
}

int CActionNetWork::sendn(SOCKET & socket, MessageStruct & MS, int flags)
{
	int isSuccess = 0;
	char* message = MS.message;
	size_t size = MS.sendRecvSize;
	isSuccess = send(socket, (char*)&size, IntSize, flags); // ������ ������
	if (isSuccess == SOCKET_ERROR)
		return CErrorHandler::ErrorHandler(ERROR_SEND);
	int temp = 0;
	while (true)
	{
		temp += send(socket, message, (int)size, flags);
		if (temp == SOCKET_ERROR)
			return CErrorHandler::ErrorHandler(ERROR_SEND);
		if (temp >= (int)size)
			break;
	}
	return SUCCES_SEND;
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
		return CErrorHandler::ErrorHandler(ERROR_RECV);
	}
	MS.sendRecvSize = *(int*)temp;
#pragma endregion
#pragma region �޼��� �ޱ�
	size_t left = MS.sendRecvSize;
	isSuccess = 0;
	while (left > 0)
	{
		isSuccess += recv(clientSocket, MS.message, (int)left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return CErrorHandler::ErrorHandler(ERROR_RECV);
		}
		else if (isSuccess >= (int)left)
			break;
	}
	MS.message[left] = '\0';
#pragma endregion
#pragma region ��ɸ޽��� �̸� ó��
	char* ptr = strchr(MS.message, '/');
	if (ptr != nullptr)
	{
		commandController.commandHandling(clientInfo, ptr);
		return SUCCES_RECV;
	}
#pragma endregion
	cout << "���� �޽��� = " << MS.message << endl;
	return SUCCES_RECV;
}

int CActionNetWork::sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags)
{
	if (nullptr == clientInfo.getMyName())
	{
		cout << "�̸� ����" << endl;
		clientInfo.setDefaultName();
	}

	size_t sendRecvSize = strlen(clientInfo.getMyName());
	char* name = clientInfo.getMyName();

	int temp = 0;
	temp = send(clientSocket, (char*)&sendRecvSize, IntSize, flags); // ������ ������
	if (temp == SOCKET_ERROR)
	{
		return CErrorHandler::ErrorHandler(ERROR_SEND);
	}
	if (IntSize == temp)
	{
		temp = 0;
		while (true)
		{
			temp += send(clientSocket, name, (int)sendRecvSize, flags);
			if (temp == SOCKET_ERROR)
			{
				return CErrorHandler::ErrorHandler(ERROR_SEND);
			}
			if (temp >= (int)sendRecvSize)
				break;
		}
	}
	return SUCCES_SEND;
}

int CActionNetWork::askClient(SOCKET & clientSocket, MessageStruct& MS, char * question)
{
	strcpy(MS.message, question);
	MS.sendRecvSize = strlen(MS.message);
	sendn(clientSocket, MS);
	if(ERROR_RECV == recvn(clientSocket, MS))
		return CErrorHandler::ErrorHandler(ERROR_RECV);
	return CErrorHandler::ErrorHandler(SUCCES_ASKCLIENT);
}

int CActionNetWork::notificationClient(SOCKET & clientSocket, MessageStruct & MS, char * notification)
{
	strcpy(MS.message, notification);
	MS.sendRecvSize = strlen(MS.message);
	sendn(clientSocket, MS);
	return CErrorHandler::ErrorHandler(SUCCES_NOTIFICATION);
}
