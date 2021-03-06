#include "ActionNetWork.h"
#include "ChannelManager.h"
#include"RoomManager.h"
#include"ReadHandler.h"
#include"graduationWork.pb.h"
using namespace graduationWork;


CActionNetWork::CActionNetWork()
{
}


CActionNetWork::~CActionNetWork()
{
	
}

int CActionNetWork::sendSize(SOCKET & sock, int size, DataType type, int flags)
{
	DataSize g_size(size, type);
	char temp[BufSize];
	int byteSize = g_size.ByteSize();
	cout << "DataSize 크기 = " << byteSize << endl;
	bool isSerialize = g_size.SerializeToArray(temp, byteSize);
	if (!isSerialize)
		return SerializeFail;

	int sendSize = 0;
	while (true)
	{
		sendSize += send(sock, temp, byteSize, flags);
		cout << "보낸 크기0 = " << sendSize << endl;
		if (sendSize >= byteSize)
			break;
	}
	cout << "보낸 크기 = " << sendSize << endl;
	return sendSize;
}

int CActionNetWork::recvSize(SOCKET & sock, DataSize& g_size, int flags)
{
	// DataSize 초기화
	g_size.Clear();
	char temp[BufSize];
	int size = 0;
	size += recv(sock, temp, recvSizeByteSize, flags);
	if (size == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return SOCKET_ERROR;
	}

	bool isParse = g_size.ParseFromArray(temp, size);
	if (!isParse)
		return ParseFail;

	return g_size.size();
}

int CActionNetWork::recvn(SOCKET & socket, Message& g_MS, int flags)
{
	char tempMessage[BufSize];
	DataSize g_size;
	int left = recvSize(socket, g_size);
#pragma endregion
#pragma region 메세지 받기
	int isSuccess = 0;
	while (left > 0)
	{
		isSuccess += recv(socket, tempMessage, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	bool isParseSucces = g_MS.ParseFromArray(tempMessage, isSuccess);
	if (!isParseSucces)
		return ParseFail;
#pragma endregion
	cout << "받은 idPw메시지 = " << g_MS.message() << endl;
//	g_MS.set_messagesize(isSuccess);
	return SuccesRecv;
}

int CActionNetWork::sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags)
{
	char temp[BufSize];
#pragma region 보낼메시지, 채널&방 번호 가져옴
	Message& g_MS = clientInfo.getMessage();
	if (&g_MS.message() == nullptr)
		return OccuredError;
	
	int size = g_MS.ByteSize();
	int channelNum = clientInfo.getMyChannelNum();
	int roomNum = clientInfo.getMyRoomNum();

	bool isSerializeSucces = g_MS.SerializeToArray(temp, size);
	if (!isSerializeSucces)
		return SerializeFail;
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
		size_t curDataSize = 0;
		// 메시지 보낸 자신이면
		if ((*iterBegin) == &clientInfo)
		{
			continue; // 보내지 않고 통과
		}

		SOCKET& clientSocket = (*iterBegin)->getClientSocket();
		sendMyName(clientSocket, clientInfo); // 이름 보내기
		sendSize(clientSocket, size, DataType::MESSAGE);
		while (true)
		{
			curDataSize += send(clientSocket, temp, size, flags);
			if (curDataSize >= size)
				break;
		}
	}
#pragma endregion
	//cout << "완료" << endl;
	return SuccesSend;
}

int CActionNetWork::sendn(SOCKET & socket, Message& g_MS, int flags)
{
	char message[BufSize];
	int size = g_MS.ByteSize();
	g_MS.SerializeToArray(message, size);
	sendSize(socket, size, DataType::MESSAGE);// 사이즈 보내기
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
	char message[BufSize];
	SOCKET& clientSocket = clientInfo.getClientSocket();
	Message& g_MS = clientInfo.getMessage();

	DataSize g_dataSize;
	int left = recvSize(clientSocket, g_dataSize);
#pragma endregion
#pragma region 메세지 받기
	int isSuccess = 0;
	while (left > 0)
	{
		isSuccess += recv(clientSocket, message, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	g_MS.ParseFromArray(message, left);
//	char* charMessage = g_MS.message().c_str();
#pragma endregion
//#pragma region 명령메시지 이면 처리
//	char* ptr = strchr(g_MS.message(), '/');
//	if (ptr != nullptr)
//	{
//		return commandController.commandHandling(clientInfo, ptr);
//	}
//#pragma endregion
	cout << "받은 메시지 = " << g_MS.message() << endl;
	return SuccesRecv;
}

int CActionNetWork::sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags)
{

	Message& myName = clientInfo.getMyNameMessage();
	int sendRecvSize = myName.ByteSize();
	
	sendSize(clientSocket, sendRecvSize, DataType::MESSAGE);

	char myNameMessage[NameSize];
	myName.SerializeToArray(myNameMessage, sendRecvSize);
	int temp = 0;
	while (true)
	{
		temp += send(clientSocket, myNameMessage, sendRecvSize, flags);
		if (temp >= sendRecvSize)
			break;
	}
	return temp;
}
