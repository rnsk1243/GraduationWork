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

int CActionNetWork::Recvn(SOCKET & socket, string& strMessage, int flags)
{
	char temp[4];
	size_t isSuccess = recv(socket, temp, IntSize, flags);

	if (isSuccess == SOCKET_ERROR)
	{
		return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
	}
	size_t sendRecvSize = *(int*)temp;

	// �ӽ÷� ���� temp �޸� ��ȯ
#pragma endregion
#pragma region �޼��� �ޱ�
	isSuccess = 0;
	char recvedMessage[BufSize];
	while (sendRecvSize > 0)
	{
		isSuccess += recv(socket, recvedMessage, (int)sendRecvSize, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
		}
		else if (isSuccess >= sendRecvSize)
			break;
	}
	recvedMessage[sendRecvSize] = '\0';
#pragma endregion
	//cout << "���� idPw�޽��� = " << MS.message << endl;
	strMessage = recvedMessage;
	return SUCCES_RECV;
}

int CActionNetWork::Sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags)
{
	int channelNum = clientInfo.GetMyChannelNum();
	int roomNum = clientInfo.GetMyRoomNum();

	vector<SOCKET> sockets;
	if (roomNum == NoneRoom)
	{
		CChannel* myChannel =  channelManager.GetMyChannel(channelNum);
		if (myChannel == nullptr)
		{
			cout << "���� ä�� ��ü�� ã�� �� �����ϴ�" << endl;
			return ErrorHandStatic->ErrorHandler(ERROR_GET_CHANNEL, &clientInfo);
		}
		myChannel->GetChannelSockets(sockets, false, &clientInfo.GetClientSocket());
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
		myRoom->GetRoomSockets(sockets, false, &clientInfo.GetClientSocket());
	}
	int sendResult = MultiSendn(sockets, clientInfo.GetMessageStruct().message);
	if (ERROR_NULL_LINK_SEND == sendResult)
	{
		return ErrorHandStatic->ErrorHandler(ERROR_SEND, &clientInfo);
	}
	else
	{
		return sendResult;
	}
	return ErrorHandStatic->ErrorHandler(ERROR_SEND, &clientInfo);
}

int CActionNetWork::Sendn(SOCKET & socket, const string & strMessage, int flags)
{
	int isSuccess = 0;
	const char* message = strMessage.c_str();
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

int CActionNetWork::Recvn(const shared_ptr<CLink>& shared_clientInfo, CCommandController& commandController, int flags)
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
		if (true == commandController.CommandHandling(shared_clientInfo, para, resultMessage, clientSocks))
		{
			int result = 0;
			if (true == clientSocks.empty())
			{
				result = Sendn(clientSocket, resultMessage);// ���ο��� 
			}
			else
			{
				result = MultiSendn(clientSocks, resultMessage); // ��ü����
			}
			if (ERROR_NULL_LINK_SEND == result)
			{
				return ERROR_NULL_LINK_SEND;
			}
			return SUCCES_RECV;
		}		
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

int CActionNetWork::MultiSendn(SocketVec& sockets, const string & strMessage, int flags)
{

	if (true == sockets.empty())
	{
		return ErrorHandStatic->ErrorHandler(ERROR_MULTI_SEND_EMPTY);
	}
	SocketVecIt iterBegin = sockets.begin();
	for (; iterBegin != sockets.end(); ++iterBegin)
	{
		if (ERROR_NULL_LINK_SEND == Sendn((*iterBegin), strMessage, flags))
		{
			return ERROR_NULL_LINK_SEND;
		}
	}
	return SUCCES_MULTI_SEND;
}
