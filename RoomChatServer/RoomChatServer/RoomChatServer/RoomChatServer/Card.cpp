#include "Card.h"



CCard::CCard()
{
}


CCard::~CCard()
{
}

CCard * CCard::getInstance()
{
	if (nullptr == CardStatic)
	{
		CardStatic = new CCard();
	}
	return CardStatic;
}

void CCard::pushCard(shared_ptr<Card> card)
{
	mCards.push_back(card);
}