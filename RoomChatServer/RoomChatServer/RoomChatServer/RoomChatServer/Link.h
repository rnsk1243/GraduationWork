//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ConstEnumInfo.h"
#include"RAII.h"
#include"GaChar.h"
#include<vector>
#include"ErrorHandler.h"

// Ŭ���̾�Ʈ�� ���������� ������ �ִ� ����ü
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
	int amount; // �� ī�忡 ���� ���� ����
	int exp; // �� ī�忡 ���� ����ġ
	int isEvolution; // �� ī�� ��ȭ ����?
	int star; // �� ��?
public:
	MyCardInfo(const shared_ptr<Card>& card_, int amount_ = 0, int exp_ = 0, int isEvolution_ = 0, int star_ = 0):
		card(card_), amount(amount_), exp(exp_), isEvolution(isEvolution_), star(star_)
	{
	}
	void increaseStar() { ++star; }
	void increaseCard() { ++amount; }
	void decreaseCard() 
	{ 
		if (amount > 0) { --amount; } 
	}
	int getAmount() { return amount; }
	int getStar() { return star; }
	int setExp(int addExp)
	{
		if (1 == isEvolution)
		{
			return CErrorHandler::ErrorHandler(ERROR_COMPOSE_EVOUTION_CARD);
		}
		exp += addExp;
		if (100 <= exp)
		{
			exp = 0;
			isEvolution = 1;
			return INFO_NEW_EVOLUTION;
		}
		return exp;
	}
	char* getCardName() { return card.get()->name; }
	const int getCardNumber() { return card.get()->cardNum; }
	const int getCardExp() { return card.get()->giveExp; }
	bool isEvoution()
	{
		if (1 == isEvolution)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	MyCardInfo(const MyCardInfo& copy) = delete;
	MyCardInfo& operator=(const MyCardInfo& copy) = delete;
};

typedef vector<shared_ptr<MyCardInfo>> MyCardVector;
typedef MyCardVector::iterator MyCardVectorIt;

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
	MyCardVector mMyCards;
	MUTEX RAII_LinkMUTEX;
	// �� ȸ����ȣ
	const int MyPKNumber;
public:
	CLink(SOCKET& clientSocket, string strPKNumber, char* name_);
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
	void PayCardGachar() { if (isMoneyOKGaChar()) { MyMoney -= CardCost; } }
	MyCardVectorIt GetIterMyCardBegin() { return mMyCards.begin(); }
	MyCardVectorIt GetIterMyCardEnd() { return mMyCards.end(); }
	bool isEmptyCard() { return mMyCards.empty(); }
	void EmptyCard();
	const int GetMyPKNumber()const { return MyPKNumber; }
#pragma endregion
	void initCard(Card* card, int amount = 0, int exp = 0, int evol = 0, int star = 0);
	void changeName(const string& newName)
	{
		size_t size = strlen(newName.c_str()) + 1;
		Name = new char[size];
		strcpy_s(Name, size, newName.c_str());
		////Name = '\0';
		//size_t char_size = strlen(name) - 1; // ��ɿ����ڸ� ������ ũ��
		//for (size_t i = 0; i < char_size; i++)
		//{
		//	Name[i] = name[i + start]; // ��ɿ����������ϰ� �ε���2���� ����
		//}
		//Name[char_size] = '\0';
		//cout << "�ٲ� �̸� = " << Name << endl;
	}
};

