//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"ConstEnumInfo.h"
#include"RAII.h"
#include"GaChar.h"

// 클라이언트가 개인적으로 가지고 있는 구조체 (개인적인 것들이 선언되어 있다...)
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
	// 현재 내가 속한 방 번호
	int MyRoomNum;
	// 현재 내가 속한 채널 번호
	int MyChannelNum;
	// 클라이언트 소켓
	SOCKET& ClientSocket;
	MessageStruct MS;
	// 나의 골드
	int MyMoney;
	MyCardList mMyCards;
	MUTEX RAII_LinkMUTEX;
public:
	CLink(SOCKET& clientSocket, char* name_);
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
		size_t char_size = strlen(name) - 1; // 명령연산자를 제외한 크기
		for (size_t i = 0; i < char_size; i++)
		{
			Name[i] = name[i + start]; // 명령연산자제외하고 인덱스2부터 복사
		}
		Name[char_size] = '\0';
		cout << "바뀐 이름 = " << Name << endl;
	}
};

