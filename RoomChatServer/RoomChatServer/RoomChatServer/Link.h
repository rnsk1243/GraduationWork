//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ConstEnumInfo.h"
#include"graduationWork.pb.h"
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
public:
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	CLink(SOCKET& clientSocket, string name_);
	~CLink();
#pragma region get, set �Լ�
	g_Message& getMessage() { return g_MS; }
	g_Transform& getTransform() { return mTransform; }
	SOCKET& getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
	g_Message& getMyNameMessage() { return g_NameMS; }
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

