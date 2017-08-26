#pragma once
#include"Card.h"

class CMyCardInfo
{
private:
	const int mCardNumber; // 카드번호
	int mAmount; // 이 카드에 대한 보유 갯수
	int mExp; // 이 카드에 대한 경험치
	int mIsEvolution; // 이 카드 진화 가능?
	int mStar; // 몇 성?
public:
	CMyCardInfo(int cardNumber, int amount = 0, int exp = 0, int isEvolution = 0, int star = 0);
	~CMyCardInfo();
	void IncreaseStar();
	void IncreaseCard();
	void DecreaseCard();
	int GetAmount();
	int GetStar();
	int GetCurExp();
	bool SetExp(int addExp, int& resultExp);
	void ResetEvolution();
	string GetCardName();
	const int GetCardNumber();
	const int GetCardExp();
	bool IsEvoution();
	CMyCardInfo(const CMyCardInfo& copy) = delete;
	CMyCardInfo& operator=(const CMyCardInfo& copy) = delete;
};
