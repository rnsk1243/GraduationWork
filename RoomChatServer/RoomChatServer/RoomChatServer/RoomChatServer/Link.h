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
	// ���� ���� ���� �� ��ȣ
	int mMyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int mMyChannelNum;
	// Ŭ���̾�Ʈ ����
	const SOCKET* mClientSocket;
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
	CLink(const SOCKET* clientSocket, const string& strPKNumber,const char* name);
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	~CLink();
	bool IsRoomEnterState();
	bool IsMoneyOKGaChar(); // ��í �� �� �ֳ�?
#pragma region get, set �Լ�
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
	bool GetPrizeBattingMoney(const int& bettingMoney); // ���øӴ� �ޱ�
	bool SaveCalculateMoney();
#pragma endregion
	void InitCard(int cardName, int amount = 0, int exp = 0, int evol = 0, int star = 0);
	bool InitGoods(int initMoney);
	bool InitBetting();
	void LostWillMoney(const int& fine); // �� �������� �߰�
	void ChangeName(const string& newName);
	////////////////////
	void SendnMine(const string& message, int flags = 0);
};

