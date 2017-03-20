#pragma once
#include<WinSock2.h>
const int BufSize = 1024;

// 클라이언트가 개인적으로 가지고 있는 구조체 (개인적인 것들이 선언되어 있다...)
struct MessageStruct
{
	char* message;
	int* sendRecvSize;
	MessageStruct():message(new char[BufSize]), sendRecvSize(new int){}
};

class CLink
{
	// 현재 내가 속한 방 번호
	int MyRoomNum;
	// 현재 내가 속한 채널 번호
	int MyChannelNum;
	// 클라이언트 소켓
	SOCKET* ClientSocket;
	MessageStruct* MS;
public:
	CLink(SOCKET* clientSocket);
	~CLink();
#pragma region get, set 함수
	MessageStruct* getMessageStruct() { return MS; }
	SOCKET* getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
#pragma endregion
};

