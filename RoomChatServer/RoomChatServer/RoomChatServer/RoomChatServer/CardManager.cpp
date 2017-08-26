#include "CardManager.h"
#include"WriteHandler.h"
#include"ReadHandler.h"
#include"Util.h"

#include"ErrorCode.h"
#include"ConstEnumInfo.h"

// 예외 안전성 기본적인 보장
// 1. 자원누수 방지
// 2. 자료구조 오염 방지

// 예외처리 완료된 함수
bool CCardManager::GetMoveCurserSize(const int& userPK, WhatCardCursorSize whatCardInfoCurser, const int & cardNum, int& resultCursorSize)
{
	//int tempResult = 0;
	int moveSize1 = 0;
	if (false == AddCipHer(userPK, moveSize1))// 회원 번호까지 누적 자릿수 더하기
	{
		ErrorHandStatic->ErrorHandler(ERROR_CURSER_SIZE);
		return false;
	}
	const int userPKnumBefore = userPK - 1;
	// 회원 번호까지 카드 템플릿 누적 마지막에 1더한 것은 pk 옆 | 때문임.
	const int moveSize2 = (userPKnumBefore * CardTemplateSize) + ((cardNum - 1) * CardInfoSize) + 1;
	const int moveCurserSize = moveSize1 + moveSize2;
	//tempResult = moveCurserSize;
	resultCursorSize = moveCurserSize;
	switch (whatCardInfoCurser)
	{
	case WhatCardCursorSize::CardNum:
		break;
	case WhatCardCursorSize::CardAmount:
		resultCursorSize += 3;
		break;
	case WhatCardCursorSize::CardExp:
		resultCursorSize += 6;
		break;
	case WhatCardCursorSize::CardEvolution:
		resultCursorSize += 9;
		break;
	case WhatCardCursorSize::CardStar:
		resultCursorSize += 11;
		break;
	default:
		return false;
	}
	//resultCursorSize = tempResult;
	return true;
}

CCardManager::CCardManager()
{
}


CCardManager::~CCardManager()
{
}

// 예외처리 완료된 함수
MyCardVectorIt CCardManager::GetMyCard(int cardNum, const LinkPtr & shared_clientInfo)
{
	CLink* targetClient = shared_clientInfo.get();
	MyCardVectorIt begin = targetClient->GetIterMyCardBegin();
	MyCardVectorIt end = targetClient->GetIterMyCardEnd();

	for (; begin != end; ++begin)
	{
		if ((*begin).get()->GetCardNumber() == cardNum)
		{
			// 카드 번호에 해당하는 반복자 리턴
			return begin;
		}
	}
	return end;
}
// 예외처리 완료된 함수
bool CCardManager::IncreaseCardAmount(int cardNum, const LinkPtr & shared_clientInfo)
{

	MyCardVectorIt cardIter = GetMyCard(cardNum, shared_clientInfo); 
	if (shared_clientInfo.get()->GetIterMyCardEnd() != cardIter)
	{
		int increasedCardAmount = (*cardIter)->GetAmount() + 1;
		if (SaveUserCardAmount(increasedCardAmount, cardNum, shared_clientInfo))
		{
			(*cardIter)->IncreaseCard(); // 쓰기 성공하면 실제 증가시킴
			return true;
		}
	}
	return false;
}
// 예외처리 완료된 함수
bool CCardManager::DecreaseCardAmount(int cardNum, const LinkPtr & shared_clientInfo)
{
	MyCardVectorIt cardIter = GetMyCard(cardNum, shared_clientInfo);
	if (shared_clientInfo.get()->GetIterMyCardEnd() != cardIter)
	{
		
		int curCardAmount = (*cardIter)->GetAmount() - 1;
		if (curCardAmount < 0)
		{
			return false;
		}
		if (SaveUserCardAmount(curCardAmount, cardNum, shared_clientInfo))
		{
			(*cardIter)->DecreaseCard();
			return true;
		}
	}

	return false;
}
// 예외처리 완료된 함수
bool CCardManager::IncreaseCardStar(int cardNum, const LinkPtr & shared_clientInfo)
{
	MyCardVectorIt cardIter = GetMyCard(cardNum, shared_clientInfo);
	if (shared_clientInfo.get()->GetIterMyCardEnd() != cardIter)
	{
		(*cardIter)->IncreaseStar();
		return true;
	}
	return false;
}
// 예외처리 완료된 함수
bool CCardManager::SaveUserCardAmount(const int& saveCardAmount, const int& cardNum, const LinkPtr & shared_clientInfo)
{
	try
	{
		int moveCurserSize = -1;
		int userPKnum = shared_clientInfo->GetMyPKNumber();
		if (true == GetMoveCurserSize(userPKnum, WhatCardCursorSize::CardAmount, cardNum, moveCurserSize))
		{
			if (true == WriteHandlerStatic->WriteObj(NameMemberCardInfoTxt, moveCurserSize, saveCardAmount, SaveCardAmountCipherTextSize))
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
// 예외 처리 완료된 함수
bool CCardManager::SaveUserCardExp(const int& saveExp, const int& cardNum, const LinkPtr & shared_clientInfo)
{
	try
	{
		int moveCurserSize = -1;
		int userPKnum = shared_clientInfo->GetMyPKNumber();
		if (true == GetMoveCurserSize(userPKnum, WhatCardCursorSize::CardExp, cardNum, moveCurserSize))
		{
			if (WriteHandlerStatic->WriteObj(NameMemberCardInfoTxt, moveCurserSize, saveExp, SaveCardExpCipherTextSize))
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
bool CCardManager::SaveUserCardEvolution(const bool & isEvolution, const int & cardNum, const LinkPtr & shared_clientInfo)
{
	try
	{
		int recordedNum = 0;
		if (isEvolution) // 진화 가능하게 바꾸고 싶으면 1을 저장하기 위해 1을 대입
		{
			recordedNum = 1;
		}
		int moveCurserSize = -1;
		int userPKnum = shared_clientInfo->GetMyPKNumber();
		if (true == GetMoveCurserSize(userPKnum, WhatCardCursorSize::CardEvolution, cardNum, moveCurserSize))
		{
			if (true == WriteHandlerStatic->WriteObj(NameMemberCardInfoTxt, moveCurserSize, recordedNum, SaveCardEvolutionCipherTextSize))
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
bool CCardManager::SaveUserCardStar(const int & saveStar, const int & cardNum, const LinkPtr & shared_clientInfo)
{
	try
	{
		int moveCurserSize = -1;
		int userPKnum = shared_clientInfo->GetMyPKNumber();
		if (true == GetMoveCurserSize(userPKnum, WhatCardCursorSize::CardStar, cardNum, moveCurserSize))
		{
			if (true == WriteHandlerStatic->WriteObj(NameMemberCardInfoTxt, moveCurserSize, saveStar, SaveCardStarCipherTextSize))
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
// 성공이면 true 반환 
bool CCardManager::ComposeCard(const LinkPtr & shared_clientInfo, int targetCard, int sourceCard)
{
	MyCardVectorIt targetCardIter = GetMyCard(targetCard, shared_clientInfo);
	MyCardVectorIt sourceCardIter = GetMyCard(sourceCard, shared_clientInfo);

	if (shared_clientInfo->GetIterMyCardEnd() != targetCardIter && shared_clientInfo->GetIterMyCardEnd() != sourceCardIter)
	{
		const int targetCardAmount = (*targetCardIter)->GetAmount();
		const int sourceCardAmount = (*sourceCardIter)->GetAmount();
		if ((targetCardAmount > 0) && (sourceCardAmount > 0))
		{
			if (true == (*targetCardIter).get()->IsEvoution()) // 진화해야 하는 카드 인가?
			{
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
				if (false == SaveUserCardEvolution(true, targetCard, shared_clientInfo))
				{
					return false;
				}
			}
			// SaveUserCardEvolution함수에서 false를 반환하지 않았다면 true가 나오므로 예외처리 안시킴
			SaveUserCardExp(resultExp, targetCard, shared_clientInfo);
			DecreaseCardAmount(sourceCard, shared_clientInfo);
			return true;
		}
	}
	return false;
}


bool CCardManager::GacharCard(const LinkPtr & shared_clientInfo)
{
	// 일단 돈 부터 깍는데 성고하면
	if (true == shared_clientInfo->PayCardGachar())
	{
		int resultCardNumber = mGacharHandler.GaCharResult(RandNumber());
		if (-1 == resultCardNumber)
		{
			return false;
		}
		if (IncreaseCardAmount(resultCardNumber, shared_clientInfo))
		{
			string resultCard("뽑은 카드 이름 : " + CardStatic->GetCardName(resultCardNumber));
			shared_clientInfo.get()->SendnMine(resultCard);
			return true;
		}
	}
	return false;
}

bool CCardManager::EvolutionCard(const LinkPtr & shared_clientInfo, int targetCard)
{
	MyCardVectorIt targetCardIter = GetMyCard(targetCard, shared_clientInfo);
	const int userPk = shared_clientInfo->GetMyPKNumber();
	
	if (shared_clientInfo->GetIterMyCardEnd() != targetCardIter)
	{
		const int cardAmount = (*targetCardIter)->GetAmount();
		if (1 < cardAmount)
		{
			if (false == (*targetCardIter).get()->IsEvoution())
			{
				return false;
			}
			IncreaseCardStar(targetCard, shared_clientInfo);
			if (true == SaveUserCardStar((*targetCardIter).get()->GetStar(), targetCard, shared_clientInfo))
			{
				(*targetCardIter).get()->ResetEvolution();
			}
			SaveUserCardEvolution(false, targetCard, shared_clientInfo);
			DecreaseCardAmount(targetCard, shared_clientInfo);
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}


