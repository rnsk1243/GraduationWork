#include "GaChar.h"
#include<random>

CGaChar::CGaChar()
{
}


CGaChar::~CGaChar()
{
}

CGaChar * CGaChar::getInstance()
{
	if (nullptr == GaCharStatic)
	{
		GaCharStatic = new CGaChar();
		return GaCharStatic;
	}
	else
	{
		return GaCharStatic;
	}
}

void CGaChar::pushCard(shared_ptr<Card> card)
{
	mCards.push_back(card);
}

int CGaChar::randNumber(int max)
{
	random_device rn;
	mt19937_64 rnd(rn());

	uniform_int_distribution<int> range(1, max);
	return range(rnd);
}

Card* CGaChar::gaCharResult(int range)
{
	list<shared_ptr<Card>>::iterator iterBegin = mCards.begin();
	list<shared_ptr<Card>>::iterator iterEnd = mCards.end();
	int maxRange = 1;
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		maxRange += (*iterBegin).get()->prob;
		if (range < maxRange)
		{
			return (*iterBegin).get();
		}
	}
	return nullptr;
}
