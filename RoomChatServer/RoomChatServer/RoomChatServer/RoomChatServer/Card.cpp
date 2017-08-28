#include "Card.h"



CCard::CCard()
{
	mCards.reserve(CardKind);
}

Card * CCard::GetCard(int cardNum)
{
	CardConstVectorIt CardIterBegin = mCards.begin();
	for (; CardIterBegin != mCards.end(); ++CardIterBegin)
	{
		if (cardNum == (*CardIterBegin).get()->mCardNum)
		{
			return (*CardIterBegin).get();
		}
	}
	return nullptr;
}


CCard::~CCard()
{
}

CCard * CCard::GetInstance()
{
	if (nullptr == CardStatic)
	{
		CardStatic = new CCard();
	}
	return CardStatic;
}

void CCard::PushCard(const CardPtr& card)
{
	cout << "pushCard = " << card.get()->mName << endl;
	mCards.push_back(card);
}

int CCard::GetCardStat(int cardNum)
{
	Card* card = GetCard(cardNum);
	if (nullptr == card)
	{
		return -1;
	}
	return card->mStat;
}

int CCard::GetCardProb(int cardNum)
{
	Card* card = GetCard(cardNum);
	if (nullptr == card)
	{
		return -1;
	}
	return card->mProb;
}

string CCard::GetCardName(int cardNum)
{
	Card* card = GetCard(cardNum);
	if (nullptr == card)
	{
		return nullptr;
	}
	return card->mName;
}

int CCard::GetCardExp(int cardNum)
{
	Card* card = GetCard(cardNum);
	if (nullptr == card)
	{
		return -1;
	}
	return card->mGiveExp;
}
