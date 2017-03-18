#pragma once
#include<WinSock2.h>
#include"Link.h"
#include"Channel.h"
#include"Room.h"
const int BufSize = 1024;

// Ŭ���̾�Ʈ�� ���������� ������ �ִ� ����ü (�������� �͵��� ����Ǿ� �ִ�...)
struct MessageStruct
{
	char* message = new char[BufSize];
	int* sendRecvSize = new int;
};

class CMyInfo
{
	// ���� ���� ���� �� ��ȣ
	int MyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int MyChannelNum;
	// Ŭ���̾�Ʈ ����
	SOCKET* ClientSocket;
	MessageStruct* MS;
public:
	CMyInfo(SOCKET* clientSocket);
	~CMyInfo();
#pragma region get, set �Լ�
	MessageStruct* getMessageStruct() { return MS; }
	SOCKET* getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
#pragma endregion
};

