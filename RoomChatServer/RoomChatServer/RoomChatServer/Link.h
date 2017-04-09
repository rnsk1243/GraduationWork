//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ConstEnumInfo.h"

// Ŭ���̾�Ʈ�� ���������� ������ �ִ� ����ü (�������� �͵��� ����Ǿ� �ִ�...)
struct MessageStruct
{
	char* message;
	size_t sendRecvSize;
	MessageStruct():message(new char[BufSize]){}
	MessageStruct& operator=(const MessageStruct& copyMS)
	{
		if (this == &copyMS)
			return *this;
		cout << "�޽��� ����" << endl;
		size_t length = strlen(copyMS.message) + 1;
		message = new char[length];
		strcpy_s(message, length, copyMS.message);
		return *this;
	}
	MessageStruct(const MessageStruct& copyMS) :sendRecvSize(copyMS.sendRecvSize)
	{
		cout << "�޽��� ����" << endl;
		size_t length = strlen(copyMS.message) + 1;
		message = new char[length];
		strcpy_s(message, length, copyMS.message);
	}
	~MessageStruct()
	{
		cout << "�޽��� �Ҹ��� ȣ��" << endl;
		delete[] message;
		message = nullptr;
	}
};

class CLink
{
	char* Name;
	// ���� ���� ���� �� ��ȣ
	int MyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int MyChannelNum;
	// Ŭ���̾�Ʈ ����
	SOCKET& ClientSocket;
	MessageStruct MS;
	MessageStruct NameMS;
	CLink(const CLink&);
	CLink& operator=(const CLink&);
public:
	CLink(SOCKET& clientSocket, char* name_);
	~CLink();
#pragma region get, set �Լ�
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
		size_t char_size = strlen(name) - 1; // ��ɿ����ڸ� ������ ũ��
		for (int i = 0; i < char_size; i++)
		{
			Name[i] = name[i + start]; // ��ɿ����������ϰ� �ε���2���� ����
		}
		Name[char_size] = '\0';
		cout << "�ٲ� �̸� = " << Name << endl;
	}
};

