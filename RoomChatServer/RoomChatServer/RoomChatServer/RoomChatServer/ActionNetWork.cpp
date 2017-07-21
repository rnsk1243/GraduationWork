#define _CRT_SECURE_NO_WARNINGS
#include "ActionNetWork.h"
#include"ErrorHandler.h"
#include "ChannelManager.h"
#include"RoomManager.h"
#include"ReadHandler.h"


CActionNetWork::CActionNetWork()
{
}


CActionNetWork::~CActionNetWork()
{
}

int CActionNetWork::Recvn(SOCKET & socket, MessageStruct& MS, int flags)
{
	char temp[4];
	size_t isSuccess = recv(socket, temp, IntSize, flags);

	if (isSuccess == SOCKET_ERROR)
	{
		return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
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
			return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
		}
		else if (isSuccess >= left)
			break;
	}
	MS.message[left] = '\0';
#pragma endregion
	cout << "���� idPw�޽��� = " << MS.message << endl;
	return SUCCES_RECV;
}

int CActionNetWork::Sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags)
{
#pragma region �����޽���, ä��&�� ��ȣ ������
	MessageStruct& MS = clientInfo.GetMessageStruct();
	char* message = MS.message;
	size_t size = MS.sendRecvSize;
	int channelNum = clientInfo.GetMyChannelNum();
	int roomNum = clientInfo.GetMyRoomNum();

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
		myChannel =  channelManager.GetMyChannel(channelNum);
		if (myChannel == nullptr)
		{
			cout << "���� ä�� ��ü�� ã�� �� �����ϴ�" << endl;
			return ErrorHandStatic->ErrorHandler(ERROR_GET_CHANNEL, &clientInfo);
		}
		// ä�� ���� ����Ʈ iter
		iterBegin = myChannel->GetIterMyInfoBegin();
		iterEnd = myChannel->GetIterMyInfoEnd();
	}
	else
	{
		CRoom* myRoom = (*(roomManager.GetMyRoomIter(channelNum, roomNum))).get();
		// �� ���� ����Ʈ iter
		if (myRoom == nullptr)
		{
			cout << "���� �水ü�� ã�� �� �����ϴ�" << endl;
			return ErrorHandStatic->ErrorHandler(ERROR_GET_ROOM, &clientInfo);
		}
		iterBegin = myRoom->GetIterMyInfoBegin();
		iterEnd = myRoom->GetIterMyInfoEnd();
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

		SOCKET& clientSocket = (*iterBegin)->GetClientSocket();

		SendMyName(clientSocket, clientInfo); // �̸� ������

		isSuccess = send(clientSocket, (char*)&size, IntSize, flags); // ������ ������
		if (isSuccess == SOCKET_ERROR)
			return ErrorHandStatic->ErrorHandler(ERROR_SEND, &clientInfo);
		while (true)
		{
			temp += send(clientSocket, message, (int)size, flags);
			if (temp == SOCKET_ERROR)
				return ErrorHandStatic->ErrorHandler(ERROR_SEND, &clientInfo);
			if (temp >= size)
				break;
		}
	}
#pragma endregion
	//cout << "�Ϸ�" << endl;
	return SUCCES_SEND;
}

int CActionNetWork::Sendn(SOCKET & socket, string & MS, int flags)
{
	int isSuccess = 0;
	const char* message = MS.c_str();
	size_t size = strlen(message);
	isSuccess = send(socket, (char*)&size, IntSize, flags); // ������ ������
	if (isSuccess == SOCKET_ERROR)
	{
		return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND);
	}
	int temp = 0;
	while (true)
	{
		temp += send(socket, message, (int)size, flags);
		if (temp == SOCKET_ERROR)
		{
			return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND);
		}
		if (temp >= (int)size)
			break;
	}
	return SUCCES_SEND;
}

int CActionNetWork::Recvn(shared_ptr<CLink> shared_clientInfo, CCommandController& commandController, int flags)
{
	CLink* clientInfo = nullptr;
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
	}
	else
	{
		return ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
	}
#pragma region ���� ������ ũ�� ��������
	char temp[4];
	SOCKET& clientSocket = clientInfo->GetClientSocket();
	MessageStruct& MS = clientInfo->GetMessageStruct();
	int isSuccess = recv(clientSocket, temp, IntSize, flags);

	if (isSuccess == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return ErrorHandStatic->ErrorHandler(ERROR_RECV, clientInfo);
	}
	MS.sendRecvSize = *(int*)temp;
#pragma endregion
#pragma region �޼��� �ޱ�
	size_t left = MS.sendRecvSize;
	isSuccess = 0;
	cout << "MS.message = " << MS.message << " // size = " << sizeof(MS.message) << endl;
	while (left > 0)
	{
		isSuccess += recv(clientSocket, MS.message, (int)left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return ErrorHandStatic->ErrorHandler(ERROR_RECV, clientInfo);
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
		string commandString = MS.message;
		cout << "��� ���� = " << commandString.c_str() << endl;
		vector<string> para = ReadHandlerStatic->Parse(commandString, '/');
		SocketVec clientSocks;
		clientSocks.reserve(EnterRoomPeopleLimit);
		string resultMessage;
		// ��� ó��
		int commandResult = commandController.CommandHandling(shared_clientInfo, para, resultMessage, clientSocks);
		//Sendn(clientSocket, resultMessage);
		int multiResult = MultiSendn(clientSocks, resultMessage);
		if (ERROR_NULL_LINK_SEND == multiResult)
		{
			return ERROR_NULL_LINK_SEND;
		}
		else if (ERROR_MULTI_SEND == multiResult)
		{
			if (ERROR_NULL_LINK_SEND == Sendn(clientSocket, resultMessage))
			{
				return ERROR_NULL_LINK_SEND;
			}
		}
		return SUCCES_RECV;
	}
#pragma endregion
	cout << "���� �޽��� = " << MS.message << endl;
	return SUCCES_RECV;
}

int CActionNetWork::SendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags)
{
	if (nullptr == clientInfo.GetMyName())
	{
		cout << "�̸� ����" << endl;
		clientInfo.SetDefaultName();
	}

	size_t sendRecvSize = strlen(clientInfo.GetMyName());
	char* name = clientInfo.GetMyName();

	int temp = 0;
	temp = send(clientSocket, (char*)&sendRecvSize, IntSize, flags); // ������ ������
	if (temp == SOCKET_ERROR)
	{
		return ErrorHandStatic->ErrorHandler(ERROR_SEND, &clientInfo);
	}
	if (IntSize == temp)
	{
		temp = 0;
		while (true)
		{
			temp += send(clientSocket, name, (int)sendRecvSize, flags);
			if (temp == SOCKET_ERROR)
			{
				return ErrorHandStatic->ErrorHandler(ERROR_SEND, &clientInfo);
			}
			if (temp >= (int)sendRecvSize)
				break;
		}
	}
	return SUCCES_SEND;
}

int CActionNetWork::AskClient(SOCKET & clientSocket, MessageStruct& MS, char * question)
{
	strcpy(MS.message, question);
	//MS.sendRecvSize = strlen(MS.message);
	string message = MS.message;
	if (ERROR_NULL_LINK_SEND == Sendn(clientSocket, message))
	{
		return ERROR_NULL_LINK_SEND;
	}
	if(ERROR_NULL_LINK_RECV == Recvn(clientSocket, MS))
		return ERROR_NULL_LINK_RECV;
	return SUCCES_ASKCLIENT;
}

int CActionNetWork::NotificationClient(SOCKET & clientSocket, MessageStruct & MS, char * notification)
{
	strcpy(MS.message, notification);
	//MS.sendRecvSize = strlen(MS.message);
	string message = MS.message;
	if (ERROR_NULL_LINK_SEND == Sendn(clientSocket, message))
	{
		return ERROR_NULL_LINK_SEND;
	}
	return SUCCES_NOTIFICATION;
}

int CActionNetWork::MultiSendn(SocketVec& sockets, string & MS, int flags)
{

	if (true == sockets.empty())
	{
		return ErrorHandStatic->ErrorHandler(ERROR_MULTI_SEND);
	}
	SocketVecIt iterBegin = sockets.begin();
	string message = MS;
	for (; iterBegin != sockets.end(); ++iterBegin)
	{
		if (ERROR_NULL_LINK_SEND == Sendn((*iterBegin), message, flags))
		{
			return ERROR_NULL_LINK_SEND;
		}
	}
	return SUCCES_MULTI_SEND;
}
