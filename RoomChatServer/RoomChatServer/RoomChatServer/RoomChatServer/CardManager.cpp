#include "CardManager.h"
#include"WriteHandler.h"
#include"ReadHandler.h"
#include"Util.h"
#include"ErrorHandler.h"
#include"ErrorCode.h"
#include"ConstEnumInfo.h"

// 예외 안전성 기본적인 보장
// 1. 자원누수 방지
// 2. 자료구조 오염 방지

// 예외처리 완료된 함수
bool CCardManager::GetMoveCurserSize(const int& userPK, WhatCardCurserSize whatCardInfoCurser, const int & cardNum, int& resultCursorSize)
{
	int tempResult = 0;
	int moveSize1 = 0;
	if (false == AddCipHer(userPK, moveSize1))// 회원 번호까지 누적 자릿수 더하기
	{
		CErrorHandler::ErrorHandler(ERROR_CURSER_SIZE);
		return false;
	}
	const int userPKnumBefore = userPK - 1;
	// 회원 번호까지 카드 템플릿 누적 마지막에 1더한 것은 pk 옆 | 때문임.
	const int moveSize2 = (userPKnumBefore * CardTemplateSize) + ((cardNum - 1) * CardInfoSize) + 1;
	const int moveCurserSize = moveSize1 + moveSize2;
	tempResult = moveCurserSize;
	switch (whatCardInfoCurser)
	{
	case WhatCardCurserSize::CardNum:
		break;
	case WhatCardCurserSize::CardAmount:
		tempResult += 3;
		break;
	case WhatCardCurserSize::CardExp:
		tempResult += 6;
		break;
	case WhatCardCurserSize::CardEvolution:
		tempResult += 9;
		break;
	case WhatCardCurserSize::CardStar:
		tempResult += 11;
		break;
	default:
		return false;
	}
	resultCursorSize = tempResult;
	return true;
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
// 예외처리 완료된 함수
MyCardVectorIt CCardManager::IsHaveCard(int cardNum, CLink& targetClient)
{
	MyCardVectorIt begin = targetClient.GetIterMyCardBegin();
	MyCardVectorIt end = targetClient.GetIterMyCardEnd();

	for (; begin != end; ++begin)
	{
		if ((*begin).get()->GetCardNumber() == cardNum)
		{
			// 카드 번호에 해당하는 반복자 리턴
			return begin;
		}
	}
	CErrorHandler::ErrorHandler(ERROR_NULL_CARD_ITERATOR);
	return end;
}
// 예외처리 완료된 함수
bool CCardManager::IncreaseCardAmount(int cardNum, CLink& targetClient)
{
	MyCardVectorIt cardIter = IsHaveCard(cardNum, targetClient); //
	if (targetClient.GetIterMyCardEnd() != cardIter)
	{
		int increasedCardAmount = (*cardIter)->GetAmount() + 1;
		if (SaveUserCardAmount(increasedCardAmount, targetClient.GetMyPKNumber(), cardNum))
		{
			(*cardIter)->IncreaseCard(); // 쓰기 성공하면 실제 증가시킴
			return true;
		}
	}
	CErrorHandler::ErrorHandler(ERROR_INCREACE_CARD);
	return false;
}
// 예외처리 완료된 함수
bool CCardManager::DecreaseCardAmount(int cardNum, CLink& targetClient)
{
	MyCardVectorIt cardIter = IsHaveCard(cardNum, targetClient);
	if (targetClient.GetIterMyCardEnd() != cardIter)
	{
		
		int curCardAmount = (*cardIter)->GetAmount() - 1;
		if (curCardAmount < 0)
		{
			CErrorHandler::ErrorHandler(ERROR_DECREACE_CARD);
			return false;
		}
		if (SaveUserCardAmount(curCardAmount, targetClient.GetMyPKNumber(), cardNum))
		{
			(*cardIter)->DecreaseCard();
			return true;
		}
	}
	CErrorHandler::ErrorHandler(ERROR_DECREACE_CARD);
	return false;
}
// 예외처리 완료된 함수
bool CCardManager::IncreaseCardStar(int cardNum, CLink & targetClient)
{
	MyCardVectorIt cardIter = IsHaveCard(cardNum, targetClient);
	if (targetClient.GetIterMyCardEnd() != cardIter)
	{
		(*cardIter)->IncreaseStar();
		int curCardStar = (*cardIter)->GetStar();
		if (SaveUserCardStar(curCardStar, targetClient.GetMyPKNumber(), cardNum))
		{
			return true;
		}
	}
	CErrorHandler::ErrorHandler(ERROR_EVOLUTION_CARD);
	return false;
}
// 예외처리 완료된 함수
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
		int moveCurserSize = -1;
		if (true == GetMoveCurserSize(userPKnum, WhatCardCurserSize::CardAmount, cardNum, moveCurserSize))
		{
			if (true == WriteHandlerStatic->WriteCard(NameMemberCardInfoTxt, moveCurserSize, saveCardAmount, SaveCardAmountCipherTextSize))
			{
				return true;
			}
		}
		return false;
	}
	catch (const std::exception&)
	{
		CErrorHandler::ErrorHandler(ERROR_SAVE_CARD);
		return false;
	}
}
// 예외 처리 완료된 함수
bool CCardManager::SaveUserCardExp(const int& saveExp, const int& userPKnum, const int& cardNum)
{
	try
	{
		int moveCurserSize = -1;
		if (true == GetMoveCurserSize(userPKnum, WhatCardCurserSize::CardExp, cardNum, moveCurserSize))
		{
			if (WriteHandlerStatic->WriteCard(NameMemberCardInfoTxt, moveCurserSize, saveExp, SaveCardExpCipherTextSize))
			{
				return true;
			}
		}
		return false;
	}
	catch (const std::exception&)
	{
		CErrorHandler::ErrorHandler(ERROR_SAVE_CARD);
		return false;
	}
}
// 예외처리 완료된 함수
bool CCardManager::SaveUserCardEvolution(const bool & isEvolution, const int & userPKnum, const int & cardNum)
{
	try
	{
		int recordedNum = 0;
		if (isEvolution) // 진화 가능하게 바꾸고 싶으면 1을 저장하기 위해 1을 대입
		{
			recordedNum = 1;
		}
		int moveCurserSize = -1;
		if (true == GetMoveCurserSize(userPKnum, WhatCardCurserSize::CardEvolution, cardNum, moveCurserSize))
		{
			if (true == WriteHandlerStatic->WriteCard(NameMemberCardInfoTxt, moveCurserSize, recordedNum, SaveCardEvolutionCipherTextSize))
			{
				return true;
			}
		}
		return false;
	}
	catch (const std::exception&)
	{
		return false;
	}
}
// 예외처리 완료된 함수
bool CCardManager::SaveUserCardStar(const int & saveStar, const int & userPKnum, const int & cardNum)
{
	try
	{
		int moveCurserSize = -1;
		if (true == GetMoveCurserSize(userPKnum, WhatCardCurserSize::CardStar, cardNum, moveCurserSize))
		{
			if (true == WriteHandlerStatic->WriteCard(NameMemberCardInfoTxt, moveCurserSize, saveStar, SaveCardStarCipherTextSize))
			{
				return true;
			}
		}
		return false;
	}
	catch (const std::exception&)
	{
		CErrorHandler::ErrorHandler(ERROR_SAVE_CARD);
		return false;
	}
}
// 성공이면 true 반환 
bool CCardManager::ComposeCard(CLink & targetClient, int targetCard, int sourceCard)
{
	MyCardVectorIt targetCardIter = IsHaveCard(targetCard, targetClient);
	MyCardVectorIt sourceCardIter = IsHaveCard(sourceCard, targetClient);
	const int userPk = targetClient.GetMyPKNumber();

	if (targetClient.GetIterMyCardEnd() != targetCardIter && targetClient.GetIterMyCardEnd() != sourceCardIter)
	{
		const int targetCardAmount = (*targetCardIter)->GetAmount();
		const int sourceCardAmount = (*sourceCardIter)->GetAmount();
		if ((targetCardAmount > 0) && (sourceCardAmount > 0))
		{
			if (true == (*targetCardIter).get()->IsEvoution()) // 진화해야 하는 카드 인가?
			{
				CErrorHandler::ErrorHandler(ERROR_COMPOSE_EVOUTION_CARD);
				return false;
			}
			int addExp = (*sourceCardIter).get()->GetCardExp();
			//const int beforeExp = (*targetCardIter).get()->GetCurExp();
			int resultExp = 0;
			// 바로 위에서 진화해야 하는 카드 인지 아닌지 검사를 했으므로 SetExp는 항상 true를 반환하게 되어있다.
			(*targetCardIter).get()->SetExp(addExp, resultExp);
			if (true == (*targetCardIter).get()->IsEvoution())
			{
				// 진화 가능 기록 함수 호출
				if (false == SaveUserCardEvolution(true, userPk, targetCard))
				{
					return false;
				}
			}
			// SaveUserCardEvolution함수에서 false를 반환하지 않았다면 true가 나오므로 예외처리 안시킴
			SaveUserCardExp(resultExp, userPk, targetCard);
			int resultSourceCardNum = 0;
			DecreaseCardAmount(sourceCard, targetClient);
			CErrorHandler::ErrorHandler(SUCCES_COMPOSE_CARD);
			return true;
		}
	}
	CErrorHandler::ErrorHandler(ERROR_COMPOSE_NULL_CARD);
	return false;
}


bool CCardManager::GacharCard(CLink & targetClient, int& resultCardNum, char*& resultCardName)
{
	Card* resultCard = mGacharHandler.GaCharResult(RandNumber());
	if (nullptr == resultCard)
	{
		CErrorHandler::ErrorHandler(ERROR_GACHAR);
		return false;
	}
	resultCardName = resultCard->mName;
	if (IncreaseCardAmount(resultCard->mCardNum, targetClient))
	{
		resultCardNum = resultCard->mCardNum;
		return true;
	}
	return false;
}

bool CCardManager::EvolutionCard(CLink & targetClient, int targetCard)
{
	MyCardVectorIt targetCardIter = IsHaveCard(targetCard, targetClient);
	const int userPk = targetClient.GetMyPKNumber();
	
	if (targetClient.GetIterMyCardEnd() != targetCardIter)
	{
		const int cardAmount = (*targetCardIter)->GetAmount();
		if (1 < cardAmount)
		{
			if (!(*targetCardIter).get()->IsEvoution())
			{
				CErrorHandler::ErrorHandler(ERROR_COMPOSE_NO_EVOUTION_CARD);
				return false;
			}
			const int curStar = IncreaseCardStar(targetCard, targetClient);
			SaveUserCardStar(curStar, userPk, targetCard);
			SaveUserCardEvolution(false, userPk, targetCard);
			DecreaseCardAmount(targetCard, targetClient);
			CErrorHandler::ErrorHandler(SUCCES_EVOLUTION_CARD);
			return true;
		}
	}
	CErrorHandler::ErrorHandler(ERROR_NULL_CARD_ITERATOR);
	return false;
}


