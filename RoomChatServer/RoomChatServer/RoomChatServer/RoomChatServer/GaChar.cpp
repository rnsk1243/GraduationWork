#include "GaChar.h"
#include<random>

CGaChar::CGaChar()
{
}


CGaChar::~CGaChar()
{
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
	CardListIt iterBegin = CardStatic->getCardListIterBegin();
	CardListIt iterEnd = CardStatic->getCardListIterEnd();
	int maxRange = 1;
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		maxRange += ((*iterBegin).get())->prob;
		if (range < maxRange)
		{
			return ((*iterBegin).get());
		}
	}
	return nullptr;
}
