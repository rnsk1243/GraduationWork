#include "GaChar.h"

CGaChar::CGaChar()
{
}


CGaChar::~CGaChar()
{
}

Card* CGaChar::GaCharResult(int range)
{
	CardVectorIt iterBegin = CardStatic->GetCardListIterBegin();
	CardVectorIt iterEnd = CardStatic->GetCardListIterEnd();
	int maxRange = 1;
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		maxRange += ((*iterBegin).get())->mProb;
		if (range < maxRange)
		{
			return ((*iterBegin).get());
		}
	}
	return nullptr;
}
