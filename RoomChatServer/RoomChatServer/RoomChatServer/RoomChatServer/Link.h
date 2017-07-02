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
	shared_ptr<Card> card;
	int amount; // 이 카드에 대한 보유 갯수
	int exp; // 이 카드에 대한 경험치
	int isEvolution; // 이 카드 진화 가능?
	int star; // 몇 성?
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
	// 현재 내가 속한 방 번호
	int MyRoomNum;
	// 현재 내가 속한 채널 번호
	int MyChannelNum;
	// 클라이언트 소켓
	SOCKET& ClientSocket;
	MessageStruct MS;
	// 나의 골드
	int MyMoney;
	MyCardVector mMyCards;
	MUTEX RAII_LinkMUTEX;
	// 내 회원번호
	const int MyPKNumber;
public:
	CLink(SOCKET& clientSocket, string strPKNumber, char* name_);
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	~CLink();
#pragma region get, set 함수
	MessageStruct& getMessageStruct() { return MS; }
	SOCKET& getClientSocket() { return ClientSocket; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	char* getMyName() { return Name; }
	void setDefaultName() { if (nullptr == Name) { Name = "이름없음"; } }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
	bool isMoneyOKGaChar() { return MyMoney >= CardCost; } // 가챠 할 수 있나?
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
		//size_t char_size = strlen(name) - 1; // 명령연산자를 제외한 크기
		//for (size_t i = 0; i < char_size; i++)
		//{
		//	Name[i] = name[i + start]; // 명령연산자제외하고 인덱스2부터 복사
		//}
		//Name[char_size] = '\0';
		//cout << "바뀐 이름 = " << Name << endl;
	}
};

