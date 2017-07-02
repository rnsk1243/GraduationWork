#include "Card.h"



CCard::CCard()
{
	mCards.reserve(CardKind);
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
	cout << "pushCard = " << card.get()->name << endl;
	mCards.push_back(card);
}