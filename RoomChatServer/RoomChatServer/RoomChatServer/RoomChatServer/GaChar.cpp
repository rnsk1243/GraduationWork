#include "GaChar.h"

CGaChar::CGaChar()
{
}


CGaChar::~CGaChar()
{
}

int CGaChar::GaCharResult(int range)
{
	int maxRange = 1;
	for (int cardNum = 1; cardNum <= CardKind; ++cardNum)
	{
		maxRange += CardStatic->GetCardProb(cardNum);
		if (range < maxRange)
		{
			return cardNum;
		}
	}
	return -1;
}
