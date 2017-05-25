//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ConstEnumInfo.h"
#include"graduationWork.pb.h"
#include"ErrorHandler.h"
using namespace graduationWork;

// 클라이언트가 개인적으로 가지고 있는 구조체 (개인적인 것들이 선언되어 있다...)
//struct MessageStruct
//{
//	char* message;
//	size_t sendRecvSize;
//	MessageStruct():message(new char[BufSize]){}
//	MessageStruct& operator=(const MessageStruct& copyMS)
//	{
//		if (this == &copyMS)
//			return *this;
//		cout << "메시지 복사" << endl;
//		size_t length = strlen(copyMS.message) + 1;
//		message = new char[length];
//		strcpy_s(message, length, copyMS.message);
//		return *this;
//	}
//	MessageStruct(const MessageStruct& copyMS) :sendRecvSize(copyMS.sendRecvSize)
//	{
//		cout << "메시지 복사" << endl;
//		size_t length = strlen(copyMS.message) + 1;
//		message = new char[length];
//		strcpy_s(message, length, copyMS.message);
//	}
//	~MessageStruct()
//	{
//		cout << "메시지 소멸자 호출" << endl;
//		delete[] message;
//		message = nullptr;
//	}
//};

struct LinkInfo
{
	// 현재 내가 속한 방 번호
	int myRoomNum;
	// 현재 내가 속한 채널 번호
	int myChannelNum;
	// 보낼 정보
	char sendData[BufSize];
	// 보낼 크기
	int size;
	// serialize성공?
	bool isSerialize;
	LinkInfo(int myRoomNum, int myChannelNum, const g_Message& gMessage) :
		myRoomNum(myRoomNum),
		myChannelNum(myChannelNum),
		size(gMessage.ByteSize()),
		isSerialize(false)
	{
		if (!gMessage.SerializeToArray(sendData, size))
		{
			CErrorHandler::ErrorHandler(ERROR_SERIALIZE_TO_ARRAY);
		}
		else
		{
			isSerialize = true;
		}
	}
	// data까지 가져오기
	LinkInfo(int myRoomNum, int myChannelNum, const g_Message& gMessage, g_DataSize& data, int clientNum = ClientNumNone) :
		myRoomNum(myRoomNum),
		myChannelNum(myChannelNum),
		size(gMessage.ByteSize()),
		isSerialize(false)
	{
		cout << "LinkInfo 생성자 = " << gMessage.message() << endl;
		if (!gMessage.SerializeToArray(sendData, size))
		{
			cout << "LinkInfo 생성 실패" << endl;
			CErrorHandler::ErrorHandler(ERROR_SERIALIZE_TO_ARRAY);
		}
		else
		{
			isSerialize = true;
		}
		cout << "size ------ = " << size << endl;
		data.set_clientnum(clientNum);
		data.set_size(size);
		data.set_type(g_DataType::MESSAGE);
	}

	LinkInfo(int myRoomNum, int myChannelNum, const g_Transform& gTransform) :
		myRoomNum(myRoomNum),
		myChannelNum(myChannelNum),
		size(gTransform.ByteSize()),
		isSerialize(false)
	{
		if (!gTransform.SerializeToArray(sendData, size))
		{
			CErrorHandler::ErrorHandler(ERROR_SERIALIZE_TO_ARRAY);
		}
		else
		{
			isSerialize = true;
		}
		g_Transform g;
		g.ParseFromArray(sendData, size);
		//cout << "gg???????? = " << g.position().x() << endl;
	}
	LinkInfo(const LinkInfo& copy): // 복사 생성자
		myRoomNum(copy.myRoomNum),
		myChannelNum(copy.myChannelNum),
		size(copy.size),
		isSerialize(copy.isSerialize)
	{
		strcpy_s(sendData, strlen(copy.sendData) + 1, copy.sendData);
	}
	LinkInfo& operator=(const LinkInfo&) = delete;
};

class CLink
{
	// 현재 내가 속한 방 번호
	int MyRoomNum;
	// 현재 내가 속한 채널 번호
	int MyChannelNum;
	// 클라이언트 소켓
	SOCKET& ClientSocket;
	g_Message g_MS;
	g_Message g_NameMS;
	g_Transform mTransform;
	char* mNameSerialize;
	g_DataSize mNameData;
	int mMyPKNumber;
public:
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	CLink(SOCKET& clientSocket, string name_, int clientNum);
	~CLink();
#pragma region get, set 함수
	g_Message& getMessage() { return g_MS; }
	//void setMessage(g_Message& message) { g_MS = message; }
	g_Transform& getTransform() { return mTransform; }
	SOCKET& getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
	g_Message& getMyNameMessage() { return g_NameMS; }
	LinkInfo getMyLinkInfoStruct(const g_Message& message)
	{
		return LinkInfo(MyRoomNum, MyChannelNum, message);
	}
	LinkInfo getMyLinkInfoStruct(const g_Transform& transform)
	{
		return LinkInfo(MyRoomNum, MyChannelNum, transform);
	}
	g_DataSize* getMyNameDataSizeType() { return &mNameData; }
	char* getMyNameSerializeData() { return mNameSerialize; }
	int getMyPKNumber() { return mMyPKNumber; }
#pragma endregion
	//void changeName(const char* name, int start)
	//{
	//	//Name = '\0';
	//	size_t char_size = strlen(name) - 1; // 명령연산자를 제외한 크기
	//	for (int i = 0; i < char_size; i++)
	//	{
	//		Name[i] = name[i + start]; // 명령연산자제외하고 인덱스2부터 복사
	//	}
	//	Name[char_size] = '\0';
	//	cout << "바뀐 이름 = " << Name << endl;
	//}
};

