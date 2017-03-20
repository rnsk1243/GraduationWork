#pragma once
#include<WinSock2.h>
const int BufSize = 1024;

// Ŭ���̾�Ʈ�� ���������� ������ �ִ� ����ü (�������� �͵��� ����Ǿ� �ִ�...)
struct MessageStruct
{
	char* message;
	int* sendRecvSize;
	MessageStruct():message(new char[BufSize]), sendRecvSize(new int){}
};

class CLink
{
	// ���� ���� ���� �� ��ȣ
	int MyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int MyChannelNum;
	// Ŭ���̾�Ʈ ����
	SOCKET* ClientSocket;
	MessageStruct* MS;
public:
	CLink(SOCKET* clientSocket);
	~CLink();
#pragma region get, set �Լ�
	MessageStruct* getMessageStruct() { return MS; }
	SOCKET* getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
#pragma endregion
};

