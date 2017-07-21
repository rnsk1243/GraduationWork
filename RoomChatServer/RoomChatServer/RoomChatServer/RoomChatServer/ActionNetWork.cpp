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

	// 임시로 만든 temp 메모리 반환
#pragma endregion
#pragma region 메세지 받기
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
	cout << "받은 idPw메시지 = " << MS.message << endl;
	return SUCCES_RECV;
}

int CActionNetWork::Sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags)
{
#pragma region 보낼메시지, 채널&방 번호 가져옴
	MessageStruct& MS = clientInfo.GetMessageStruct();
	char* message = MS.message;
	size_t size = MS.sendRecvSize;
	int channelNum = clientInfo.GetMyChannelNum();
	int roomNum = clientInfo.GetMyRoomNum();

	//cout << "보낼 메세지 = " << message << endl;
	//cout << "보낼 사이즈 = " << size << endl;
#pragma endregion
#pragma region 전달자 및 나의 방 담을 임시 변수
	LinkListIt iterBegin;
	LinkListIt iterEnd;

	CChannel* myChannel = nullptr;
	CRoom* myRoom = nullptr;
#pragma endregion

#pragma region 내가 속한 채널 방 가져오기
	if (roomNum == NoneRoom)
	{
		myChannel =  channelManager.GetMyChannel(channelNum);
		if (myChannel == nullptr)
		{
			cout << "나의 채널 객체를 찾을 수 없습니다" << endl;
			return ErrorHandStatic->ErrorHandler(ERROR_GET_CHANNEL, &clientInfo);
		}
		// 채널 소켓 리스트 iter
		iterBegin = myChannel->GetIterMyInfoBegin();
		iterEnd = myChannel->GetIterMyInfoEnd();
	}
	else
	{
		CRoom* myRoom = (*(roomManager.GetMyRoomIter(channelNum, roomNum))).get();
		// 방 소켓 리스트 iter
		if (myRoom == nullptr)
		{
			cout << "나의 방객체를 찾을 수 없습니다" << endl;
			return ErrorHandStatic->ErrorHandler(ERROR_GET_ROOM, &clientInfo);
		}
		iterBegin = myRoom->GetIterMyInfoBegin();
		iterEnd = myRoom->GetIterMyInfoEnd();
	}
#pragma endregion
#pragma region 내가 속한 채널 or 방에게 보내기
	int isSuccess = 0;
	// 방에 있는 모든 사람에게 보내기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		size_t temp = 0;
		// 메시지 보낸 자신이면
		if ((*iterBegin).get() == &clientInfo)
		{
			continue; // 보내지 않고 통과
		}

		SOCKET& clientSocket = (*iterBegin)->GetClientSocket();

		SendMyName(clientSocket, clientInfo); // 이름 보내기

		isSuccess = send(clientSocket, (char*)&size, IntSize, flags); // 사이즈 보내기
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
	//cout << "완료" << endl;
	return SUCCES_SEND;
}

int CActionNetWork::Sendn(SOCKET & socket, string & MS, int flags)
{
	int isSuccess = 0;
	const char* message = MS.c_str();
	size_t size = strlen(message);
	isSuccess = send(socket, (char*)&size, IntSize, flags); // 사이즈 보내기
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
#pragma region 받을 데이터 크기 가져오기
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
#pragma region 메세지 받기
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
#pragma region 명령메시지 이면 처리
	
	char* ptr = strchr(MS.message, '/');
	if (ptr != nullptr)
	{
		string commandString = MS.message;
		cout << "명령 내용 = " << commandString.c_str() << endl;
		vector<string> para = ReadHandlerStatic->Parse(commandString, '/');
		SocketVec clientSocks;
		clientSocks.reserve(EnterRoomPeopleLimit);
		string resultMessage;
		// 명령 처리
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
	cout << "받은 메시지 = " << MS.message << endl;
	return SUCCES_RECV;
}

int CActionNetWork::SendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags)
{
	if (nullptr == clientInfo.GetMyName())
	{
		cout << "이름 없음" << endl;
		clientInfo.SetDefaultName();
	}

	size_t sendRecvSize = strlen(clientInfo.GetMyName());
	char* name = clientInfo.GetMyName();

	int temp = 0;
	temp = send(clientSocket, (char*)&sendRecvSize, IntSize, flags); // 사이즈 보내기
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
