#include "GaChar.h"

CGaChar::CGaChar()
{
}


CGaChar::~CGaChar()
{
}

Card* CGaChar::gaCharResult(int range)
{
	CardVectorIt iterBegin = CardStatic->getCardListIterBegin();
	CardVectorIt iterEnd = CardStatic->getCardListIterEnd();
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
