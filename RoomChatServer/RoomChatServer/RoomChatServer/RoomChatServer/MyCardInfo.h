#pragma once
#include"Card.h"

class CMyCardInfo
{
private:
	const int mCardNumber; // ī���ȣ
	int mAmount; // �� ī�忡 ���� ���� ����
	int mExp; // �� ī�忡 ���� ����ġ
	int mIsEvolution; // �� ī�� ��ȭ ����?
	int mStar; // �� ��?
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
