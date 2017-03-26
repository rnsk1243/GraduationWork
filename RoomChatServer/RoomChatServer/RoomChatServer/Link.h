#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
const int BufSize = 1024;
const int NameSize = 30;

// 클라이언트가 개인적으로 가지고 있는 구조체 (개인적인 것들이 선언되어 있다...)
struct MessageStruct
{
	char* message;
	int* sendRecvSize;
	MessageStruct():message(new char[BufSize]), sendRecvSize(new int){}
	MessageStruct(const MessageStruct& copyMS) :message(new char[BufSize]), sendRecvSize(new int)
	{
		cout << "메시지 복사" << endl;
		strcpy_s(message, BufSize, copyMS.message);
		*sendRecvSize = *copyMS.sendRecvSize;
	}
};

class CLink
{
	char* Name;
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
	CLink& operator=(const CLink& link)
	{
		if (this == &link)
			return *this;
	}
#pragma region get, set 함수
	MessageStruct* getMessageStruct() { return MS; }
	SOCKET* getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	char* getMyName() { return Name; }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
#pragma endregion
	void changeName(const char* name, int start)
	{
		//Name = '\0';
		int char_size = strlen(name) - 1; // 명령연산자를 제외한 크기
		for (int i = 0; i < char_size; i++)
		{
			Name[i] = name[i + start]; // 명령연산자제외하고 인덱스2부터 복사
		}
		Name[char_size] = '\0';
		cout << "바뀐 이름 = " << Name << endl;
	}
};

