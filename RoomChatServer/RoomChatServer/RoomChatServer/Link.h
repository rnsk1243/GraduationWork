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
	int sendRecvSize;
	MessageStruct():
		message(new char[BufSize]),
		sendRecvSize(0)
	{}
	MessageStruct(const MessageStruct& ms):
		sendRecvSize(ms.sendRecvSize)
	{
		if (ms.message)
		{
			int length = strlen(ms.message) + 1;
			message = new char[length];
			for (int i = 0; i < length - 1; i++)
			{
				message[i] = ms.message[i];
			}
			message[length - 1] = '\0';
		}
	}
	~MessageStruct()
	{
		cout << "메세지 구조체 소멸자 호출" << endl;
		delete message;
	}
};

class CLink
{
	// 내 고유번호
	int MyPKNum;
	char* Name;
	// 현재 내가 속한 방 번호
	int MyRoomNum;
	// 현재 내가 속한 채널 번호
	int MyChannelNum;
	// 클라이언트 소켓
	SOCKET& ClientSocket; // &로 선언한 이유는 클래스 생성시 바로 초기화 해줄 것이고
	// 종료될때까지 바뀌지 않는 값이기 때문
	MessageStruct& MS;
public:
	CLink(SOCKET& clientSocket, MessageStruct& MS, int myPKNum);
	~CLink();
	// 복사 생성자
	CLink(const CLink& link) :
		MyRoomNum(link.MyRoomNum),
		MyChannelNum(link.MyChannelNum),
		ClientSocket(link.ClientSocket),
		MS(link.MS),
		MyPKNum(link.MyPKNum)
	{
		if (link.Name)
		{
			int length = strlen(link.Name) + 1;
			Name = new char[length];
			for (int i = 0; i < length - 1; i++)
			{
				Name[i] = link.Name[i];
			}
			Name[length - 1] = '\0';
		}

	}
	CLink& operator = (const CLink& link)
	{
		if (this == &link)
			return *this;
		else 
		{
			if (link.Name)
			{
				if (Name)
				{
					delete Name;
					Name = nullptr;
				}
				int length = strlen(link.Name) + 1;
				Name = new char[length];
				for (int i = 0; i < length - 1; i++)
				{
					Name[i] = link.Name[i];
				}
				Name[length - 1] = '\0';
			}
			MyRoomNum = link.MyChannelNum;
			MyChannelNum = link.MyChannelNum;
			ClientSocket = link.ClientSocket;
			MS = link.MS;
			return *this;
		}
	}
	bool operator<(const CLink& other)
	{
		if (MyPKNum > other.MyPKNum)
			return true;
		else
			return false;
	}
#pragma region get, set 함수
	MessageStruct& getMessageStruct() { return MS; }
	SOCKET& getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	char* getMyName() { return Name; }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
	int getMyPKNum() { return MyPKNum; }
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

