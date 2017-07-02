#include "CardManager.h"
#include"WriteHandler.h"
#include"ReadHandler.h"
#include"Util.h"
#include"ErrorHandler.h"
#include"ErrorCode.h"
#include"ConstEnumInfo.h"

int CCardManager::GetMoveCurserSize(const int& userPK, WhatCardCurserSize whatCardInfoCurser, const int & cardNum)
{
	int resultCurserSize = 0;

	const int moveSize1 = AddCipHer(userPK); // 회원 번호까지 누적 자릿수 더하기
	const int userPKnumBefore = userPK - 1;
	// 회원 번호까지 카드 템플릿 누적 마지막에 1더한 것은 pk 옆 | 때문임.
	const int moveSize2 = (userPKnumBefore * CardTemplateSize) + ((cardNum - 1) * CardInfoSize) + 1; 
	const int moveCurserSize = moveSize1 + moveSize2;

	resultCurserSize = moveCurserSize;
	switch (whatCardInfoCurser)
	{
	case WhatCardCurserSize::CardNum:
		return resultCurserSize;
	case WhatCardCurserSize::CardAmount:
		resultCurserSize += 3;
		return resultCurserSize;
	case WhatCardCurserSize::CardExp:
		resultCurserSize += 6;
		return resultCurserSize;
	case WhatCardCurserSize::CardEvolution:
		resultCurserSize += 9;
		return resultCurserSize;
	case WhatCardCurserSize::CardStar:
		resultCurserSize += 11;
		return resultCurserSize;
	default:
		break;
	}
	return CErrorHandler::ErrorHandler(ERROR_CURSER_SIZE);
}

CCardManager::CCardManager()
{
}


CCardManager::~CCardManager()
{
}

//const int CCardManager::getCardAmount(const char* textName, char* userName, int cardNum)
//{
//	bool isNewCard = true;
//	int cardAmount = 0;
//	char targetLine[BufSize];
//	char chCardNum[3];
//	string strUserName = userName;
//	int line = ReadHandlerStatic->Search(textName, 1, strUserName);
//	WriteHandlerStatic->BeginToTargetUserLineCurserMoveSize(textName, line, targetLine);
//	IntToAlphabet(cardNum, chCardNum);
//	string strCardNum = chCardNum;
//	WriteHandlerStatic->TargetLineToUserCurserMoveSize(targetLine, strCardNum, isNewCard, cardAmount);
//	return cardAmount;
//}

int CCardManager::IsHaveCard(int cardNum, MyCardVectorIt& cardIter, CLink& targetClient)
{
	MyCardVectorIt begin = targetClient.GetIterMyCardBegin();
	MyCardVectorIt end = targetClient.GetIterMyCardEnd();
	int amount = 0;

	for (; begin != end; ++begin)
	{
		if ((*begin).get()->getCardNumber() == cardNum)
		{
			cardIter = begin;
			amount = (*begin)->getAmount();
			break;
		}
	}
	if ((*cardIter) == nullptr)
	{
		return CErrorHandler::ErrorHandler(ERROR_NULL_CARD_ITERATOR);
	}
	return amount;
}

int CCardManager::IncreaseCardAmount(int cardNum, CLink& targetClient)
{
	MyCardVectorIt cardIter;
	if (ERROR_NULL_CARD_ITERATOR != IsHaveCard(cardNum, cardIter, targetClient))
	{
		(*cardIter)->increaseCard();
		int curCardAmount = (*cardIter)->getAmount();
		if (SaveUserCardAmount(curCardAmount, targetClient.GetMyPKNumber(), cardNum))
		{
			return (*cardIter)->getAmount();
		}
	}
	return CErrorHandler::ErrorHandler(ERROR_INCREACE_CARD);
}

int CCardManager::DecreaseCardAmount(int cardNum, CLink& targetClient)
{
	MyCardVectorIt cardIter;
	if (ERROR_NULL_CARD_ITERATOR != IsHaveCard(cardNum, cardIter, targetClient))
	{
		(*cardIter)->decreaseCard();
		int curCardAmount = (*cardIter)->getAmount();
		if (SaveUserCardAmount(curCardAmount, targetClient.GetMyPKNumber(), cardNum))
		{
			return (*cardIter)->getAmount();
		}
	}
	return CErrorHandler::ErrorHandler(ERROR_DECREACE_CARD);
}

int CCardManager::IncreaseCardStar(int cardNum, CLink & targetClient)
{
	MyCardVectorIt cardIter;
	if (ERROR_NULL_CARD_ITERATOR != IsHaveCard(cardNum, cardIter, targetClient))
	{
		(*cardIter)->increaseStar();
		int curCardStar = (*cardIter)->getStar();
		if (SaveUserCardStar(curCardStar, targetClient.GetMyPKNumber(), cardNum))
		{
			return (*cardIter)->getStar();
		}
	}
	return CErrorHandler::ErrorHandler(ERROR_EVOLUTION_CARD);
}

bool CCardManager::SaveUserCardAmount(const int& saveCardAmount, const int& userPKnum, const int& cardNum)
{
	try
	{
		//char targetLine[BufSize];
		//char chCardNum[3];
		//bool isNewCard = true;
		//int cardAmount = 0;
		//string strUserName = userName;
		//int line = ReadHandlerStatic->Search(textName, 1, strUserName);
		//int moveSize1 = WriteHandlerStatic->BeginToTargetUserLineCurserMoveSize(textName, line, targetLine);
		//IntToAlphabet(cardNum, chCardNum);
		//string strCardNum = chCardNum;
		//int movesize2 = WriteHandlerStatic->TargetLineToUserCurserMoveSize(targetLine, strCardNum, isNewCard, cardAmount);
		//if (0 > cardAmount && isIncrease == false)
		//{
		//	CErrorHandler::ErrorHandler(ERROR_DECREASE_CARD);
		//	return false;
		//}

		const int moveCurserSize = GetMoveCurserSize(userPKnum, WhatCardCurserSize::CardAmount, cardNum);
		WriteHandlerStatic->WriteCard(NameMemberCardInfoTxt, moveCurserSize, saveCardAmount);
		return true;
	}
	catch (const std::exception&)
	{
		CErrorHandler::ErrorHandler(ERROR_SAVE_CARD);
		return false;
	}
}

bool CCardManager::SaveUserCardExp(const int& saveExp, const int& userPKnum, const int& cardNum)
{
	try
	{
		const int moveCurserSize = GetMoveCurserSize(userPKnum, WhatCardCurserSize::CardExp, cardNum);
		WriteHandlerStatic->WriteCard(NameMemberCardInfoTxt, moveCurserSize, saveExp, 2);
		return true;
	}
	catch (const std::exception&)
	{
		CErrorHandler::ErrorHandler(ERROR_SAVE_CARD);
		return false;
	}
}

bool CCardManager::SaveUserCardEvolution(const bool & isEvolution, const int & userPKnum, const int & cardNum)
{
	try
	{
		int recordedNum = 0;
		if (isEvolution)
		{
			recordedNum = 1;
		}
		const int moveCurserSize = GetMoveCurserSize(userPKnum, WhatCardCurserSize::CardEvolution, cardNum);
		WriteHandlerStatic->WriteCard(NameMemberCardInfoTxt, moveCurserSize, recordedNum, 1);
		return true;
	}
	catch (const std::exception&)
	{
		CErrorHandler::ErrorHandler(ERROR_SAVE_CARD);
		return false;
	}
}

bool CCardManager::SaveUserCardStar(const int & saveStar, const int & userPKnum, const int & cardNum)
{
	try
	{

		const int moveCurserSize = GetMoveCurserSize(userPKnum, WhatCardCurserSize::CardStar, cardNum);
		WriteHandlerStatic->WriteCard(NameMemberCardInfoTxt, moveCurserSize, saveStar, 1);
		return true;
	}
	catch (const std::exception&)
	{
		CErrorHandler::ErrorHandler(ERROR_SAVE_CARD);
		return false;
	}
}

int CCardManager::ComposeCard(CLink & targetClient, int targetCard, int sourceCard)
{
	MyCardVectorIt targetCardIter;
	MyCardVectorIt sourceCardIter;
	const int userPk = targetClient.GetMyPKNumber();
	//이터레이터 없애기
	const int targetCardAmount = IsHaveCard(targetCard, targetCardIter, targetClient);
	const int sourceCardAmount = IsHaveCard(sourceCard, sourceCardIter, targetClient);
	if (ERROR_NULL_CARD_ITERATOR != targetCardAmount && ERROR_NULL_CARD_ITERATOR != sourceCardAmount)
	{
		if ((targetCardAmount > 0) && (sourceCardAmount > 0))
		{
			if ((*targetCardIter).get()->isEvoution())
			{
				return CErrorHandler::ErrorHandler(ERROR_COMPOSE_EVOUTION_CARD);
			}
			int addExp = (*sourceCardIter).get()->getCardExp();
			int resultExp = (*targetCardIter).get()->setExp(addExp);

			if ((*targetCardIter).get()->isEvoution())
			{
				// 진화 가능 기록 함수 호출
				SaveUserCardEvolution(true, userPk, targetCard);
				resultExp = 0;
			}
			SaveUserCardExp(resultExp, userPk, targetCard);
			DecreaseCardAmount(sourceCard, targetClient);
			return resultExp;
		}
	}
	return CErrorHandler::ErrorHandler(ERROR_COMPOSE_NULL_CARD);
}


int CCardManager::GacharCard(CLink & targetClient, char*& resultCardName)
{
	Card* resultCard = mGacharHandler.GaCharResult(RandNumber());
	if (nullptr == resultCard)
	{
		return CErrorHandler::ErrorHandler(ERROR_GACHAR);
	}
	resultCardName = resultCard->mName;
	int curCardAmount = IncreaseCardAmount(resultCard->mCardNum, targetClient);
	return resultCard->mCardNum;
}

int CCardManager::EvolutionCard(CLink & targetClient, int targetCard)
{
	MyCardVectorIt targetCardIter;
	const int userPk = targetClient.GetMyPKNumber();
	const int cardAmount = IsHaveCard(targetCard, targetCardIter, targetClient);
	if (ERROR_NULL_CARD_ITERATOR != cardAmount)
	{
		if (1 < cardAmount)
		{
			if (!(*targetCardIter).get()->isEvoution())
			{
				return CErrorHandler::ErrorHandler(ERROR_COMPOSE_NO_EVOUTION_CARD);
			}
			const int curSatr = IncreaseCardStar(targetCard, targetClient);
			SaveUserCardStar(curSatr, userPk, targetCard);
			SaveUserCardEvolution(false, userPk, targetCard);
			DecreaseCardAmount(targetCard, targetClient);
			return curSatr;
		}
	}
	return ERROR_NULL_CARD_ITERATOR;
}


