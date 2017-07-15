//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
#include<vector>
using namespace std;
#include"ConstEnumInfo.h"
#include"RAII.h"
#include"GaChar.h"
#include"Goods.h"
//class CErrorHandler;
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
	~MyCardInfo()
	{

	}
	void IncreaseStar() { ++mStar; }
	void IncreaseCard() { ++mAmount; }
	void DecreaseCard() 
	{ 
		if (mAmount > 0) { --mAmount; } 
	}
	int GetAmount() { return mAmount; }
	int GetStar() { return mStar; }
	int GetCurExp() { return mExp; }
	bool SetExp(int addExp, int& resultExp);
	char* GetCardName() { return mCard.get()->mName; }
	const int GetCardNumber() { return mCard.get()->mCardNum; }
	const int GetCardExp() { return mCard.get()->mGiveExp; }
	bool IsEvoution()
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
private:
	char* mName;
	// ���� ���� ���� �� ��ȣ
	int mMyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int mMyChannelNum;
	// Ŭ���̾�Ʈ ����
	SOCKET& mClientSocket;
	MessageStruct mMS;
	// ���� ��ȭ
	CGoods mMyGoods;
	MyCardVector mMyCards;
	MUTEX mRAII_LinkMUTEX;
	// �� ȸ����ȣ
	const int mMyPKNumber;
	bool mIsInitCards; // ī�� �ʱ�ȭ �߳�?
	bool mIsInitGoods; // ��ȭ �ʱ�ȭ �߳�?
	bool InitMoney(int money) { return mMyGoods.InitMoney(money); }
	bool AddMoney(const int& addMoney) { return mMyGoods.AddMyMoney(addMoney, this); }
	bool MinusMyMoney(const int& minusMoney) { return mMyGoods.MinusMyMoney(minusMoney, this); }
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
	bool IsMoneyOKGaChar() { return GetMyMoney() >= CardCost; } // ��í �� �� �ֳ�?
	bool PayCardGachar() { if (IsMoneyOKGaChar()) { return MinusMyMoney(CardCost); } }
	MyCardVectorIt GetIterMyCardBegin() { return mMyCards.begin(); }
	MyCardVectorIt GetIterMyCardEnd() { return mMyCards.end(); }
	bool IsEmptyCard() { return mMyCards.empty(); }
	bool IsZeroMoney() { return mMyGoods.IsZeroMoney(); }
	bool SetZeroMoney() { return mMyGoods.SetZeroMoney(this); }
	const int GetMyMoney() { return mMyGoods.GetMyMoney(); }
	void EmptyCard();
	const int GetMyPKNumber()const { return mMyPKNumber; }
#pragma endregion
	void InitCard(Card* card, int amount = 0, int exp = 0, int evol = 0, int star = 0);
	bool InitGoods(int initMoney);
	void SetInitCards() { mIsInitCards = true; }
	void SetInitGoods() { mIsInitGoods = true; }
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

