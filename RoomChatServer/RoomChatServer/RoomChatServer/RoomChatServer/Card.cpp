#include "Card.h"



CCard::CCard()
{
	mCards.reserve(CardKind);
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

void CCard::PushCard(shared_ptr<Card> card)
{
	cout << "pushCard = " << card.get()->mName << endl;
	mCards.push_back(card);
}