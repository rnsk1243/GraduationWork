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
// 클라이언트가 개인적으로 가지고 있는 구조체
struct MessageStruct
{
	char* message;
	size_t sendRecvSize;
	MessageStruct() :message(new char[BufSize]),sendRecvSize(0)
	{
		cout << "메시지 생성자 호출 주소 = " << &message << endl; 
	}
	MessageStruct& operator=(const MessageStruct& copyMS)
	{
		if (this == &copyMS)
			return *this;
		cout << "메시지 복사" << endl;
		size_t length = strlen(copyMS.message) + 1;
		message = new char[length];
		strcpy_s(message, length, copyMS.message);
		return *this;
	}
	MessageStruct(const MessageStruct& copyMS) :sendRecvSize(copyMS.sendRecvSize)
	{
		cout << "메시지 복사" << endl;
		size_t length = strlen(copyMS.message) + 1;
		message = new char[length];
		strcpy_s(message, length, copyMS.message);
	}
	~MessageStruct()
	{
		cout << " 메시지 소멸자 호출 소멸될 메시지 주소 = " << &message << "// 값 = " << message << endl;
		delete[] message;
		message = nullptr;
	}
};

class MyCardInfo
{
private:
	const int mCardNumber; // 카드번호
	int mAmount; // 이 카드에 대한 보유 갯수
	int mExp; // 이 카드에 대한 경험치
	int mIsEvolution; // 이 카드 진화 가능?
	int mStar; // 몇 성?
public:
	MyCardInfo(int cardNumber, int amount = 0, int exp = 0, int isEvolution = 0, int star = 0):
		mCardNumber(cardNumber), mAmount(amount), mExp(exp), mIsEvolution(isEvolution), mStar(star)
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
	char* GetCardName() { return CardStatic->GetCardName(mCardNumber); }
	const int GetCardNumber() { return mCardNumber; }
	const int GetCardExp() { return CardStatic->GetCardExp(mCardNumber); }
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
	// 현재 내가 속한 방 번호
	int mMyRoomNum;
	// 현재 내가 속한 채널 번호
	int mMyChannelNum;
	// 클라이언트 소켓
	SOCKET& mClientSocket;
	MessageStruct mMS;
	// 나의 재화
	CGoods mMyGoods;
	MyCardVector mMyCards;
	MUTEX mRAII_LinkMUTEX;
	// 내 회원번호
	const int mMyPKNumber;
	// 게임할 준비 됐나?
	bool mIsGameOK;
	// 내가 배팅에 낸 카드번호
	int mBattingCardNum;
	bool mIsInitCards; // 카드 초기화 했나?
	bool mIsInitGoods; // 재화 초기화 했나?
	bool InitMoney(int money);
	bool AddMoney(const int& addMoney);
	bool MinusMyMoney(const int& minusMoney);
	bool IsHaveCard(int cardNum);
public:
	CLink(SOCKET& clientSocket,const string& strPKNumber,const char* name);
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	~CLink();
#pragma region get, set 함수
	MessageStruct& GetMessageStruct() { return mMS; }
	SOCKET& GetClientSocket() { return mClientSocket; }
	int GetMyRoomNum() { return mMyRoomNum; }
	int GetMyChannelNum() { return mMyChannelNum; }
	char* GetMyName() { return mName; }
	void SetDefaultName() { if (nullptr == mName) { mName = "이름없음"; } }
	void SetMyRoomNum(int myRoomNum) { mMyRoomNum = myRoomNum; }
	void SetMyChannelNum(int myChannelNum) { mMyChannelNum = myChannelNum; }
	bool IsMoneyOKGaChar() { return GetMyMoney() >= CardCost; } // 가챠 할 수 있나?
	bool PayCardGachar();
	MyCardVectorIt GetIterMyCardBegin() { return mMyCards.begin(); }
	MyCardVectorIt GetIterMyCardEnd() { return mMyCards.end(); }
	bool IsEmptyCard() { return mMyCards.empty(); }
	bool IsZeroMoney() { return mMyGoods.IsZeroMoney(); }
	bool SetZeroMoney();
	const int GetMyMoney() { return mMyGoods.GetMyMoney(); }
	void EmptyCard();
	const int GetMyPKNumber()const { return mMyPKNumber; }
	void SetInitCards() { mIsInitCards = true; }
	void SetInitGoods() { mIsInitGoods = true; }
	void SetReadyGame() { mIsGameOK = true; }
	void SetNoReadyGame() { mIsGameOK = false; }
	bool GetReadyGame() { return mIsGameOK; }
	bool SetMyBattingCard(int cardNum);
	bool GetReadyBatting();
	int GetMyBattingCardNumber();
#pragma endregion
	void InitCard(int cardName, int amount = 0, int exp = 0, int evol = 0, int star = 0);
	bool InitGoods(int initMoney);
	void ChangeName(const string& newName)
	{
		size_t size = strlen(newName.c_str()) + 1;
		mName = new char[size];
		strcpy_s(mName, size, newName.c_str());
		////Name = '\0';
		//size_t char_size = strlen(name) - 1; // 명령연산자를 제외한 크기
		//for (size_t i = 0; i < char_size; i++)
		//{
		//	Name[i] = name[i + start]; // 명령연산자제외하고 인덱스2부터 복사
		//}
		//Name[char_size] = '\0';
		//cout << "바뀐 이름 = " << Name << endl;
	}
};

