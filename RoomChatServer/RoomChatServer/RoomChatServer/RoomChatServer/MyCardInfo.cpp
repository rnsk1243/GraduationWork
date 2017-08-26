#include "MyCardInfo.h"



CMyCardInfo::CMyCardInfo(int cardNumber, int amount, int exp, int isEvolution, int star) :
	mCardNumber(cardNumber), mAmount(amount), mExp(exp), mIsEvolution(isEvolution), mStar(star)
{
}


CMyCardInfo::~CMyCardInfo()
{
}

void CMyCardInfo::IncreaseStar()
{
	++mStar;
}

void CMyCardInfo::IncreaseCard()
{
	++mAmount;
}

void CMyCardInfo::DecreaseCard()
{
	if (mAmount > 0) { --mAmount; }
}

int CMyCardInfo::GetAmount()
{
	return mAmount;
}

int CMyCardInfo::GetStar()
{
	return mStar;
}

int CMyCardInfo::GetCurExp()
{
	return mExp;
}

bool CMyCardInfo::SetExp(int addExp, int & resultExp)
{
	if (1 == mIsEvolution)
	{
		return false;
	}
	mExp += addExp;
	if (100 <= mExp)
	{
		mExp = 0;
		mIsEvolution = 1;
	}
	resultExp = mExp;
	return true;
}

void CMyCardInfo::ResetEvolution()
{
	mIsEvolution = 0;
}

string CMyCardInfo::GetCardName()
{
	return CardStatic->GetCardName(mCardNumber);
}

const int CMyCardInfo::GetCardNumber()
{
	return mCardNumber;
}

const int CMyCardInfo::GetCardExp()
{
	return CardStatic->GetCardExp(mCardNumber);
}

bool CMyCardInfo::IsEvoution()
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
