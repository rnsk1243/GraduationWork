#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
const int BufSize = 1024;
const int NameSize = 30;

// Ŭ���̾�Ʈ�� ���������� ������ �ִ� ����ü (�������� �͵��� ����Ǿ� �ִ�...)
struct MessageStruct
{
	char* message;
	int* sendRecvSize;
	MessageStruct():message(new char[BufSize]), sendRecvSize(new int){}
	MessageStruct(const MessageStruct& copyMS) :message(new char[BufSize]), sendRecvSize(new int)
	{
		cout << "�޽��� ����" << endl;
		strcpy_s(message, BufSize, copyMS.message);
		*sendRecvSize = *copyMS.sendRecvSize;
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
#pragma region get, set �Լ�
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
		int char_size = strlen(name) - 1; // ��ɿ����ڸ� ������ ũ��
		for (int i = 0; i < char_size; i++)
		{
			Name[i] = name[i + start]; // ��ɿ����������ϰ� �ε���2���� ����
		}
		Name[char_size] = '\0';
		cout << "�ٲ� �̸� = " << Name << endl;
	}
};

