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
	char* mName;
	// ���� ���� ���� �� ��ȣ
	int mMyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int mMyChannelNum;
	// Ŭ���̾�Ʈ ����
	SOCKET& mClientSocket;
	//MessageStruct mMS;
	// ���� ��ȭ
	CGoods mMyGoods;
	MyCardVector mMyCards;
	MUTEX mRAII_LinkMUTEX;
	// �� ȸ����ȣ
	const int mMyPKNumber;
	// ������ �غ� �Ƴ�?
	bool mIsGameOK;
	// ���� ���ÿ� �� ī���ȣ
	int mBattingCardNum;
	int mDebtMoney;	// ������ ������ ��.
	int mPayBackMoney; // ������ ���� ��.
	bool mIsInitCards; // ī�� �ʱ�ȭ �߳�?
	bool mIsInitGoods; // ��ȭ �ʱ�ȭ �߳�?
	bool InitMoney(int money);
	bool AddMoney(const int& addMoney);
	bool MinusMyMoney(const int& minusMoney);
	bool IsHaveCard(int cardNum);
	bool PayBackMoney(const int& payBack);
public:
	CLink(SOCKET& clientSocket,const string& strPKNumber,const char* name);
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	~CLink();
#pragma region get, set �Լ�
	//MessageStruct& GetMessageStruct() { return mMS; }
	SOCKET& GetClientSocket() { return mClientSocket; }
	int GetMyRoomNum() { return mMyRoomNum; }
	bool IsRoomEnterState();
	int GetMyChannelNum() { return mMyChannelNum; }
	char* GetMyName() { return mName; }
	void SetDefaultName() { if (nullptr == mName) { mName = "�̸�����"; } }
	void SetMyRoomNum(int myRoomNum) { mMyRoomNum = myRoomNum; }
	void SetMyChannelNum(int myChannelNum) { mMyChannelNum = myChannelNum; }
	bool IsMoneyOKGaChar() { return GetMyMoney() >= CardCost; } // ��í �� �� �ֳ�?
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
	bool GetPrizeBattingMoney(const int bettingMoney); // ���øӴ� �ޱ�
	bool SaveCalculateMoney();
#pragma endregion
	void InitCard(int cardName, int amount = 0, int exp = 0, int evol = 0, int star = 0);
	bool InitGoods(int initMoney);
	bool InitBetting();
	bool RefundBettingMoney(const int& bettingMoney);
	bool FineGamePlayingOut();
	void ChangeName(const string& newName)
	{
		size_t size = strlen(newName.c_str()) + 1;
		mName = new char[size];
		strcpy_s(mName, size, newName.c_str());
	}
	////////////////////
	void SendnMine(const string& message, int flags = 0);
};

