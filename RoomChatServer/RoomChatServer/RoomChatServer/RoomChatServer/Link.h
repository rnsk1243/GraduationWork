//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ConstEnumInfo.h"
#include"RAII.h"
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

class MyCardInfo
{
private:
	shared_ptr<Card> card;
	int amount;
	float exp;
public:
	MyCardInfo(const shared_ptr<Card>& card_, int amount_ = 1, float exp_ = 0.0f):
		card(card_), amount(amount_), exp(exp_)
	{
	}
	void increaseCard() { ++amount; }
	int getAmount() { return amount; }
	char* getCardName() { return card.get()->name; }
	MyCardInfo(const MyCardInfo& copy) = delete;
	MyCardInfo& operator=(const MyCardInfo& copy) = delete;
};

typedef list<shared_ptr<MyCardInfo>> MyCardList;
typedef MyCardList::iterator MyCardListIt;

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
	MyCardList mMyCards;
	MUTEX RAII_LinkMUTEX;
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
	MyCardListIt GetIterMyCardBegin() { return mMyCards.begin(); }
	MyCardListIt GetIterMyCardEnd() { return mMyCards.end(); }
	bool isEmptyCard() { return mMyCards.empty(); }
	void EmptyCard();
	bool isHaveCard(int cardNum, MyCardListIt& cardIter);
#pragma endregion
	void pushCard(Card* card);
	void initCard(Card* card, int amount = 1, float exp = 0.0f);
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

