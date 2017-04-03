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
		cout << "�޼��� ����ü �Ҹ��� ȣ��" << endl;
		delete message;
	}
};

class CLink
{
	// �� ������ȣ
	int MyPKNum;
	char* Name;
	// ���� ���� ���� �� ��ȣ
	int MyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int MyChannelNum;
	// Ŭ���̾�Ʈ ����
	SOCKET& ClientSocket; // &�� ������ ������ Ŭ���� ������ �ٷ� �ʱ�ȭ ���� ���̰�
	// ����ɶ����� �ٲ��� �ʴ� ���̱� ����
	MessageStruct& MS;
public:
	CLink(SOCKET& clientSocket, MessageStruct& MS, int myPKNum);
	~CLink();
	// ���� ������
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
#pragma region get, set �Լ�
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
		int char_size = strlen(name) - 1; // ��ɿ����ڸ� ������ ũ��
		for (int i = 0; i < char_size; i++)
		{
			Name[i] = name[i + start]; // ��ɿ����������ϰ� �ε���2���� ����
		}
		Name[char_size] = '\0';
		cout << "�ٲ� �̸� = " << Name << endl;
	}
};

