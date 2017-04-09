//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ConstEnumInfo.h"

// 클라이언트가 개인적으로 가지고 있는 구조체 (개인적인 것들이 선언되어 있다...)
struct MessageStruct
{
	char* message;
	size_t sendRecvSize;
	MessageStruct():message(new char[BufSize]){}
	MessageStruct& operator=(const MessageStruct& copyMS)
	{
		if (this == &copyMS)
			return *this;
		cout << "메시지 복사" << endl;
		size_t length = strlen(copyMS.message) + 1;
		message = new char[length];
		strcpy_s(message, length, copyMS.message);
		return *this;
	}
	MessageStruct(const MessageStruct& copyMS) :sendRecvSize(copyMS.sendRecvSize)
	{
		cout << "메시지 복사" << endl;
		size_t length = strlen(copyMS.message) + 1;
		message = new char[length];
		strcpy_s(message, length, copyMS.message);
	}
	~MessageStruct()
	{
		cout << "메시지 소멸자 호출" << endl;
		delete[] message;
		message = nullptr;
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
	SOCKET& ClientSocket;
	MessageStruct MS;
	MessageStruct NameMS;
	CLink(const CLink&);
	CLink& operator=(const CLink&);
public:
	CLink(SOCKET& clientSocket, char* name_);
	~CLink();
#pragma region get, set 함수
	MessageStruct& getMessageStruct() { return MS; }
	SOCKET& getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	char* getMyName() { return Name; }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
	MessageStruct& getMyNameMessageStruct() { return NameMS; }
#pragma endregion
	void changeName(const char* name, int start)
	{
		//Name = '\0';
		size_t char_size = strlen(name) - 1; // 명령연산자를 제외한 크기
		for (int i = 0; i < char_size; i++)
		{
			Name[i] = name[i + start]; // 명령연산자제외하고 인덱스2부터 복사
		}
		Name[char_size] = '\0';
		cout << "바뀐 이름 = " << Name << endl;
	}
};

