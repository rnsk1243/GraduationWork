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
#pragma region 보낼메시지, 채널&방 번호 가져옴
	MessageStruct* MS = clientInfo->getMessageStruct();
	if (MS->message == nullptr)
		return OccuredError;
	char* message = MS->message;
	int* size = MS->sendRecvSize;
	int channelNum = clientInfo->getMyChannelNum();
	int roomNum = clientInfo->getMyRoomNum();

	cout << "보낼 메세지 = " << message << endl;
	cout << "보낼 사이즈 = " << *size << endl;
#pragma endregion
#pragma region 전달자 및 나의 방 담을 임시 변수
	LinkListIt iterBegin;
	LinkListIt iterEnd;

	CChannel* myChannel;
	CRoom* myRoom;
#pragma endregion

#pragma region 내가 속한 채널 방 가져오기
	if (roomNum == NoneRoom)
	{
		myChannel =  roomChannelManager->getMyChannel(channelNum);
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
		CRoom* myRoom = *(roomChannelManager->getMyRoomIter(channelNum, roomNum));
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
		int temp = 0;
		// 메시지 보낸 자신이면
		if ((*iterBegin) == clientInfo)
		{
			continue; // 보내지 않고 통과
		}
		SOCKET* clientSocket = (*iterBegin)->getClientSocket();
		send(*clientSocket, (char*)size, IntSize, flags); // 사이즈 보내기
		while (true)
		{
			temp += send(*clientSocket, message, *size, flags);
			if (temp >= *size)
				break;
		}
	}
#pragma endregion
	//cout << "완료" << endl;
	return SuccesSend;
}

int CSendRecv::recvn(CLink* clientInfo, CCommandController* commandController, int flags)
{
#pragma region 받을 데이터 크기 가져오기
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

	// 임시로 만든 temp 메모리 반환
	delete temp;
#pragma endregion
#pragma region 메세지 받기
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
#pragma region 명령메시지 이면 처리
	char* ptr = strchr(MS->message, '/');
	if (ptr != nullptr)
	{
		return commandController->commandHandling(clientInfo, ptr);
	}
#pragma endregion
	cout << "받은 메시지 = " << MS->message << endl;
	return SuccesRecv;
}