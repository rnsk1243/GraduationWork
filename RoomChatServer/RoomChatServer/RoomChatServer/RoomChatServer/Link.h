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

// 클라이언트가 개인적으로 가지고 있는 구조체
struct MessageStruct
{
	char* message;
	size_t sendRecvSize;
	MessageStruct() :message(new char[BufSize]) { cout << "메시지 생성자 호출 주소 = " << &message << endl; }
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
	shared_ptr<Card> mCard;
	int mAmount; // 이 카드에 대한 보유 갯수
	int mExp; // 이 카드에 대한 경험치
	int mIsEvolution; // 이 카드 진화 가능?
	int mStar; // 몇 성?
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
	// 현재 내가 속한 방 번호
	int mMyRoomNum;
	// 현재 내가 속한 채널 번호
	int mMyChannelNum;
	// 클라이언트 소켓
	SOCKET& mClientSocket;
	MessageStruct mMS;
	// 나의 골드
	int mMyMoney;
	MyCardVector mMyCards;
	MUTEX mRAII_LinkMUTEX;
	// 내 회원번호
	const int mMyPKNumber;
public:
	CLink(SOCKET& clientSocket, string strPKNumber, char* name);
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
	bool IsMoneyOKGaChar() { return mMyMoney >= CardCost; } // 가챠 할 수 있나?
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
		//size_t char_size = strlen(name) - 1; // 명령연산자를 제외한 크기
		//for (size_t i = 0; i < char_size; i++)
		//{
		//	Name[i] = name[i + start]; // 명령연산자제외하고 인덱스2부터 복사
		//}
		//Name[char_size] = '\0';
		//cout << "바뀐 이름 = " << Name << endl;
	}
};

