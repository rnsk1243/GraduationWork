//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ConstEnumInfo.h"
#include"graduationWork.pb.h"
#include"ErrorHandler.h"
using namespace graduationWork;

// Ŭ���̾�Ʈ�� ���������� ������ �ִ� ����ü (�������� �͵��� ����Ǿ� �ִ�...)
//struct MessageStruct
//{
//	char* message;
//	size_t sendRecvSize;
//	MessageStruct():message(new char[BufSize]){}
//	MessageStruct& operator=(const MessageStruct& copyMS)
//	{
//		if (this == &copyMS)
//			return *this;
//		cout << "�޽��� ����" << endl;
//		size_t length = strlen(copyMS.message) + 1;
//		message = new char[length];
//		strcpy_s(message, length, copyMS.message);
//		return *this;
//	}
//	MessageStruct(const MessageStruct& copyMS) :sendRecvSize(copyMS.sendRecvSize)
//	{
//		cout << "�޽��� ����" << endl;
//		size_t length = strlen(copyMS.message) + 1;
//		message = new char[length];
//		strcpy_s(message, length, copyMS.message);
//	}
//	~MessageStruct()
//	{
//		cout << "�޽��� �Ҹ��� ȣ��" << endl;
//		delete[] message;
//		message = nullptr;
//	}
//};

struct LinkInfo
{
	// ���� ���� ���� �� ��ȣ
	int myRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int myChannelNum;
	// ���� ����
	char sendData[BufSize];
	// ���� ũ��
	int size;
	// serialize����?
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
	// data���� ��������
	LinkInfo(int myRoomNum, int myChannelNum, const g_Message& gMessage, g_DataSize& data, int clientNum = ClientNumNone) :
		myRoomNum(myRoomNum),
		myChannelNum(myChannelNum),
		size(gMessage.ByteSize()),
		isSerialize(false)
	{
		cout << "LinkInfo ������ = " << gMessage.message() << endl;
		if (!gMessage.SerializeToArray(sendData, size))
		{
			cout << "LinkInfo ���� ����" << endl;
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
	LinkInfo(const LinkInfo& copy): // ���� ������
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
	// ���� ���� ���� �� ��ȣ
	int MyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int MyChannelNum;
	// Ŭ���̾�Ʈ ����
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
#pragma region get, set �Լ�
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
	//	size_t char_size = strlen(name) - 1; // ��ɿ����ڸ� ������ ũ��
	//	for (int i = 0; i < char_size; i++)
	//	{
	//		Name[i] = name[i + start]; // ��ɿ����������ϰ� �ε���2���� ����
	//	}
	//	Name[char_size] = '\0';
	//	cout << "�ٲ� �̸� = " << Name << endl;
	//}
};

