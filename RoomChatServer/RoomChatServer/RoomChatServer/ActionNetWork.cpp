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

	// 임시로 만든 temp 메모리 반환
#pragma endregion
#pragma region 메세지 받기
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
	cout << "받은 idPw메시지 = " << MS.message << endl;
	return SuccesRecv;
}

int CActionNetWork::sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags)
{
#pragma region 보낼메시지, 채널&방 번호 가져옴
	MessageStruct& MS = clientInfo.getMessageStruct();
	if (MS.message == nullptr)
		return OccuredError;
	char* message = MS.message;
	size_t size = MS.sendRecvSize;
	int channelNum = clientInfo.getMyChannelNum();
	int roomNum = clientInfo.getMyRoomNum();

	cout << "보낼 메세지 = " << message << endl;
	cout << "보낼 사이즈 = " << size << endl;
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
		myChannel =  channelManager.getMyChannel(channelNum);
		if (myChannel == nullptr)
		{
			cout << "나의 채널 객체를 찾을 수 없습니다" << endl;
			return OccuredError;
		}
		// 채널 소켓 리스트 iter
		iterBegin = myChannel->getIterMyInfoBegin();
		iterEnd = myChannel->getIterMyInfoEnd();
	}
	else
	{
		CRoom* myRoom = *(roomManager.getMyRoomIter(channelNum, roomNum));
		// 방 소켓 리스트 iter
		if (myRoom == nullptr)
		{
			cout << "나의 방객체를 찾을 수 없습니다" << endl;
			return OccuredError;
		}
		iterBegin = myRoom->getIterMyInfoBegin();
		iterEnd = myRoom->getIterMyInfoEnd();
	}
#pragma endregion
#pragma region 내가 속한 채널 or 방에게 보내기
	// 방에 있는 모든 사람에게 보내기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		size_t temp = 0;
		// 메시지 보낸 자신이면
		if ((*iterBegin) == &clientInfo)
		{
			continue; // 보내지 않고 통과
		}

		SOCKET& clientSocket = (*iterBegin)->getClientSocket();

		sendMyName(clientSocket, clientInfo); // 이름 보내기

		send(clientSocket, (char*)&size, IntSize, flags); // 사이즈 보내기
		while (true)
		{
			temp += send(clientSocket, message, size, flags);
			if (temp >= size)
				break;
		}
	}
#pragma endregion
	//cout << "완료" << endl;
	return SuccesSend;
}

int CActionNetWork::sendn(SOCKET & socket, MessageStruct & MS, int flags)
{
	char* message = MS.message;
	size_t size = MS.sendRecvSize;
	send(socket, (char*)&size, IntSize, flags); // 사이즈 보내기
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
#pragma region 받을 데이터 크기 가져오기
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
#pragma region 메세지 받기
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
#pragma region 명령메시지 이면 처리
	char* ptr = strchr(MS.message, '/');
	if (ptr != nullptr)
	{
		return commandController.commandHandling(clientInfo, ptr);
	}
#pragma endregion
	cout << "받은 메시지 = " << MS.message << endl;
	return SuccesRecv;
}

int CActionNetWork::sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags)
{
	if (nullptr == clientInfo.getMyName())
	{
		cout << "이름 없음" << endl;
		return NullNameError;
	}
	MessageStruct& myName = clientInfo.getMyNameMessageStruct();
	myName.message = clientInfo.getMyName();
	myName.sendRecvSize = strlen(myName.message);

	cout << "보낼 메세지 = " << myName.message << endl;
	cout << "보낼 사이즈 = " << myName.sendRecvSize << endl;

	int temp = 0;
	temp = send(clientSocket, (char*)&myName.sendRecvSize, IntSize, flags); // 사이즈 보내기
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
