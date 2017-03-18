#include "NetWork.h"



CNetWork::CNetWork(CLink* link, CMyInfo* myInfo, CChannelHandlere* channelHandlere, CRoomHandlere* roomHandlere)
	:Link(link), ClientInfo(myInfo), ChannelHandlere(channelHandlere), RoomHandlere(roomHandlere)
{
}


CNetWork::~CNetWork()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
	deleteClientSocket();// client소켓을 우선 방or채널 리스트에서 지우고 나서
	delete ClientInfo;
}

int CNetWork::sendn(int flags)
{
#pragma region 보낼메시지, 채널&방 번호 가져옴
	MessageStruct* MS = ClientInfo->getMessageStruct();
	if (MS->message == nullptr)
		return OccuredError;
	char* message = MS->message;
	int* size = MS->sendRecvSize;
	int channelNum = ClientInfo->getMyChannelNum();
	int roomNum = ClientInfo->getMyRoomNum();

	cout << "보낼 메세지 = " << message << endl;
	cout << "보낼 사이즈 = " << *size << endl;
#pragma endregion
#pragma region 전달자 및 나의 방 담을 임시 변수
	SocketListIt iterBegin;
	SocketListIt iterEnd;

	CChannel* myChannel;
	CRoom* myRoom;
#pragma endregion

#pragma region 내가 속한 채널 방 가져오기
	if (roomNum == NoneRoom)
	{
		myChannel = Link->getMyChannel(channelNum);
		if (myChannel == nullptr)
		{
			cout << "나의 채널 객체를 찾을 수 없습니다" << endl;
			return OccuredError;
		}
		// 채널 소켓 리스트 iter
		iterBegin = myChannel->getIterBegin();
		iterEnd = myChannel->getIterEnd();
	}
	else
	{
		CRoom* myRoom = *(Link->getMyRoomIter(channelNum, roomNum));
		// 방 소켓 리스트 iter
		if (myRoom == nullptr)
		{
			cout << "나의 방객체를 찾을 수 없습니다" << endl;
			return OccuredError;
		}
		iterBegin = myRoom->getIterSocketBegin();
		iterEnd = myRoom->getIterSocketEnd();
	}
#pragma endregion
#pragma region 내가 속한 채널 or 방에게 보내기
	// 방에 있는 모든 사람에게 보내기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		int temp = 0;
		// 메시지 보낸 자신이면
		if ((*iterBegin) == ClientInfo->getClientSocket())
		{
			continue; // 보내지 않고 통과
		}
		send(*(*iterBegin), (char*)size, IntSize, flags); // 사이즈 보내기
		while (true)
		{
			temp += send(*(*iterBegin), message, *size, flags);
			if (temp >= *size)
				break;
		}
	}
#pragma endregion
	//cout << "완료" << endl;
	return SuccesSend;
}

int CNetWork::recvn(int flags)
{
#pragma region 받을 데이터 크기 가져오기
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
		return commandHandling(ptr);
	}
#pragma endregion
	cout << "받은 메시지 = " << MS->message << endl;
	return SuccesRecv;
}

bool CNetWork::deleteClientSocket()
{
	int myChannelNum = ClientInfo->getMyChannelNum();
	int myRoomNum = ClientInfo->getMyRoomNum();
	//방에 있나 채널에 있나 확인
	if (NoneRoom == myRoomNum)
	{
		// 채널일때
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
		// 방일때
		RoomListIt myRoomIter = Link->getMyRoomIter(myChannelNum, myRoomNum);
		CRoom* myRoom = (*myRoomIter);
		SocketListIt socketBegin = myRoom->getIterSocketBegin();
		SocketListIt socketEnd = myRoom->getIterSocketEnd();
		for (; socketBegin != socketEnd; ++socketBegin)
		{
			if ((*socketBegin) == ClientInfo->getClientSocket())
			{
				ClientInfo->setMyRoomNum(NoneRoom); // 방에 속하지 않음
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
	cout << "명령 처리 시작" << endl;
	int channelNum = ClientInfo->getMyChannelNum();
	int roomNum = ClientInfo->getMyRoomNum();
	*command++;
#pragma region 명령처리
	if (*command == 'e') // 방에 입장
	{
		bool isEnterSucces = false;
#pragma region 개설된 모든 방
		RoomListIt roomBegin = Link->getIterRoomBegin();
		RoomListIt roomEnd = Link->getIterRoomEnd();
#pragma endregion
#pragma region 내가 입장 할 수 있는 방 찾기
		for (; roomBegin != roomEnd; ++roomBegin)
		{
			CRoom* room = *roomBegin;
			if (room->getChannelNum() == channelNum)
			{
				if (room->getAmountPeople() < EnterRoomPeopleLimit)
				{
					cout << "방에 입장" << endl;
					RoomHandlere->enterRoom(ClientInfo, Link, room->getRoomNum());
					// 채널에서는 나가기
					ChannelHandlere->exitChannel(ClientInfo, Link);
					isEnterSucces = true;
				}
			}
		}
#pragma endregion
		if(!isEnterSucces)
			cout << "입장 할 수 있는 방이 없습니다." << endl;
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
				cout << moveChannelNum << "번 채널 변경" << endl;
			}
		}
	}
	else if (*command == 'm')
	{
		//원래 채널에서는 나가기
		ChannelHandlere->exitChannel(ClientInfo, Link);
		cout << "방 만들기" << endl;
		char* roomName = RoomHandlere->returnRoomName(command);
		RoomHandlere->makeRoom(ClientInfo, Link, roomName);
	}
	else if (*command == 'o')
	{
		// 다시 채널로 돌아가고
		ChannelHandlere->enterChannel(ClientInfo, Link, channelNum);
		cout << "방에서 나가기" << endl;
		RoomHandlere->exitRoom(ClientInfo, Link);

	}
#pragma endregion
	return SuccesCommand;
}
