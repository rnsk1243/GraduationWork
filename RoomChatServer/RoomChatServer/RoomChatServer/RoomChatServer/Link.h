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
#include"MyCardInfo.h"

typedef shared_ptr<CLink> LinkPtr;
typedef vector<shared_ptr<CMyCardInfo>> MyCardVector;
typedef MyCardVector::iterator MyCardVectorIt;

class CLink
{
private:
	string mName;
	// 현재 내가 속한 방 번호
	int mMyRoomNum;
	// 현재 내가 속한 채널 번호
	int mMyChannelNum;
	// 클라이언트 소켓
	const SOCKET* mClientSocket;
	//MessageStruct mMS;
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
	int mDebtMoney;	// 앞으로 없어질 돈.
	int mPayBackMoney; // 앞으로 받을 돈.
	bool mIsInitCards; // 카드 초기화 했나?
	bool mIsInitGoods; // 재화 초기화 했나?
	bool InitMoney(int money);
	bool AddMoney(const int& addMoney);
	bool MinusMyMoney(const int& minusMoney);
	bool IsHaveCard(int cardNum);
	bool PayBackMoney(const int& payBack);
public:
	CLink(const SOCKET* clientSocket, const string& strPKNumber,const char* name);
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	~CLink();
	bool IsRoomEnterState();
	bool IsMoneyOKGaChar(); // 가챠 할 수 있나?
#pragma region get, set 함수
	const SOCKET* GetClientSocket();
	int GetMyRoomNum();
	void SetMyRoomNum(const int& myRoomNum);
	int GetMyChannelNum();
	void SetMyChannelNum(const int& myChannelNum);
	string GetMyName();
	bool PayCardGachar();
	MyCardVectorIt GetIterMyCardBegin();
	MyCardVectorIt GetIterMyCardEnd();
	bool IsEmptyCard();
	bool IsZeroMoney();
	void SetZeroMoney();
	const int GetMyMoney();
	void EmptyCard();
	const int GetMyPKNumber()const;
	void SetInitCards();
	void SetInitGoods();
	void SetReadyGame(const int& battingMoney);
	void SetNoReadyGame();
	bool GetReadyGame();
	bool SetMyBattingCard(int cardNum);
	bool GetReadyBatting();
	int GetMyBattingCardNumber();
	bool GetPrizeBattingMoney(const int& bettingMoney); // 베팅머니 받기
	bool SaveCalculateMoney();
#pragma endregion
	void InitCard(int cardName, int amount = 0, int exp = 0, int evol = 0, int star = 0);
	bool InitGoods(int initMoney);
	bool InitBetting();
	void LostWillMoney(const int& fine); // 곧 없어질돈 추가
	void ChangeName(const string& newName);
	////////////////////
	void SendnMine(const string& message, int flags = 0);
};

