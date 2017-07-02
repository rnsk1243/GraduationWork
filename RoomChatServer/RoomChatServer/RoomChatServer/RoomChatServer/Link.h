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
	shared_ptr<Card> mCard;
	int mAmount; // �� ī�忡 ���� ���� ����
	int mExp; // �� ī�忡 ���� ����ġ
	int mIsEvolution; // �� ī�� ��ȭ ����?
	int mStar; // �� ��?
public:
	MyCardInfo(const shared_ptr<Card>& card, int amount = 0, int exp = 0, int isEvolution = 0, int star = 0):
		mCard(card), mAmount(amount), mExp(exp), mIsEvolution(isEvolution), mStar(star)
	{
	}
	void increaseStar() { ++mStar; }
	void increaseCard() { ++mAmount; }
	void decreaseCard() 
	{ 
		if (mAmount > 0) { --mAmount; } 
	}
	int getAmount() { return mAmount; }
	int getStar() { return mStar; }
	int setExp(int addExp)
	{
		if (1 == mIsEvolution)
		{
			return CErrorHandler::ErrorHandler(ERROR_COMPOSE_EVOUTION_CARD);
		}
		mExp += addExp;
		if (100 <= mExp)
		{
			mExp = 0;
			mIsEvolution = 1;
			return INFO_NEW_EVOLUTION;
		}
		return mExp;
	}
	char* getCardName() { return mCard.get()->mName; }
	const int getCardNumber() { return mCard.get()->mCardNum; }
	const int getCardExp() { return mCard.get()->mGiveExp; }
	bool isEvoution()
	{
		if (1 == mIsEvolution)
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
	char* mName;
	// ���� ���� ���� �� ��ȣ
	int mMyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int mMyChannelNum;
	// Ŭ���̾�Ʈ ����
	SOCKET& mClientSocket;
	MessageStruct mMS;
	// ���� ���
	int mMyMoney;
	MyCardVector mMyCards;
	MUTEX mRAII_LinkMUTEX;
	// �� ȸ����ȣ
	const int mMyPKNumber;
public:
	CLink(SOCKET& clientSocket, string strPKNumber, char* name);
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	~CLink();
#pragma region get, set �Լ�
	MessageStruct& GetMessageStruct() { return mMS; }
	SOCKET& GetClientSocket() { return mClientSocket; }
	int GetMyRoomNum() { return mMyRoomNum; }
	int GetMyChannelNum() { return mMyChannelNum; }
	char* GetMyName() { return mName; }
	void SetDefaultName() { if (nullptr == mName) { mName = "�̸�����"; } }
	void SetMyRoomNum(int myRoomNum) { mMyRoomNum = myRoomNum; }
	void SetMyChannelNum(int myChannelNum) { mMyChannelNum = myChannelNum; }
	bool IsMoneyOKGaChar() { return mMyMoney >= CardCost; } // ��í �� �� �ֳ�?
	void PayCardGachar() { if (IsMoneyOKGaChar()) { mMyMoney -= CardCost; } }
	MyCardVectorIt GetIterMyCardBegin() { return mMyCards.begin(); }
	MyCardVectorIt GetIterMyCardEnd() { return mMyCards.end(); }
	bool IsEmptyCard() { return mMyCards.empty(); }
	void EmptyCard();
	const int GetMyPKNumber()const { return mMyPKNumber; }
#pragma endregion
	void InitCard(Card* card, int amount = 0, int exp = 0, int evol = 0, int star = 0);
	void ChangeName(const string& newName)
	{
		size_t size = strlen(newName.c_str()) + 1;
		mName = new char[size];
		strcpy_s(mName, size, newName.c_str());
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

