//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ConstEnumInfo.h"

#include"GaChar.h"
// Ŭ���̾�Ʈ�� ���������� ������ �ִ� ����ü (�������� �͵��� ����Ǿ� �ִ�...)
struct MessageStruct
{
	char* message;
	size_t sendRecvSize;
	MessageStruct() :message(new char[BufSize]) { cout << "�޽��� ������ ȣ�� �ּ� = " << &message << endl; }
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
		cout << " �޽��� �Ҹ��� ȣ�� �Ҹ�� �޽��� �ּ� = " << &message << "// �� = " << message << endl;
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
	// ���� ���
	int MyMoney;
	list<Card*> mMyCards;
public:
	CLink(SOCKET& clientSocket, char* name_);
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	~CLink();
#pragma region get, set �Լ�
	MessageStruct& getMessageStruct() { return MS; }
	SOCKET& getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	char* getMyName() { return Name; }
	void setDefaultName() { if (nullptr == Name) { Name = "�̸�����"; } }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
	bool isMoneyOKGaChar() { return MyMoney >= CardCost; } // ��í �� �� �ֳ�?
#pragma endregion
	void pushCard(Card* card);
	void changeName(const char* name, int start)
	{
		//Name = '\0';
		size_t char_size = strlen(name) - 1; // ��ɿ����ڸ� ������ ũ��
		for (size_t i = 0; i < char_size; i++)
		{
			Name[i] = name[i + start]; // ��ɿ����������ϰ� �ε���2���� ����
		}
		Name[char_size] = '\0';
		cout << "�ٲ� �̸� = " << Name << endl;
	}
};

