#include "CardManager.h"
#include"WriteHandler.h"
#include"ReadHandler.h"
#include"Util.h"

#include"ErrorCode.h"
#include"ConstEnumInfo.h"

// ���� ������ �⺻���� ����
// 1. �ڿ����� ����
// 2. �ڷᱸ�� ���� ����

// ����ó�� �Ϸ�� �Լ�
bool CCardManager::GetMoveCurserSize(const int& userPK, WhatCardCursorSize whatCardInfoCurser, const int & cardNum, int& resultCursorSize)
{
	//int tempResult = 0;
	int moveSize1 = 0;
	if (false == AddCipHer(userPK, moveSize1))// ȸ�� ��ȣ���� ���� �ڸ��� ���ϱ�
	{
		ErrorHandStatic->ErrorHandler(ERROR_CURSER_SIZE);
		return false;
	}
	const int userPKnumBefore = userPK - 1;
	// ȸ�� ��ȣ���� ī�� ���ø� ���� �������� 1���� ���� pk �� | ������.
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

// ����ó�� �Ϸ�� �Լ�
MyCardVectorIt CCardManager::GetMyCard(int cardNum, const LinkPtr & shared_clientInfo)
{
	CLink* targetClient = shared_clientInfo.get();
	MyCardVectorIt begin = targetClient->GetIterMyCardBegin();
	MyCardVectorIt end = targetClient->GetIterMyCardEnd();

	for (; begin != end; ++begin)
	{
		if ((*begin).get()->GetCardNumber() == cardNum)
		{
			// ī�� ��ȣ�� �ش��ϴ� �ݺ��� ����
			return begin;
		}
	}
	return end;
}
// ����ó�� �Ϸ�� �Լ�
bool CCardManager::IncreaseCardAmount(int cardNum, const LinkPtr & shared_clientInfo)
{

	MyCardVectorIt cardIter = GetMyCard(cardNum, shared_clientInfo); 
	if (shared_clientInfo.get()->GetIterMyCardEnd() != cardIter)
	{
		int increasedCardAmount = (*cardIter)->GetAmount() + 1;
		if (SaveUserCardAmount(increasedCardAmount, cardNum, shared_clientInfo))
		{
			(*cardIter)->IncreaseCard(); // ���� �����ϸ� ���� ������Ŵ
			return true;
		}
	}
	return false;
}
// ����ó�� �Ϸ�� �Լ�
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
// ����ó�� �Ϸ�� �Լ�
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
// ����ó�� �Ϸ�� �Լ�
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
// ���� ó�� �Ϸ�� �Լ�
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
// ����ó�� �Ϸ�� �Լ�
bool CCardManager::SaveUserCardEvolution(const bool & isEvolution, const int & cardNum, const LinkPtr & shared_clientInfo)
{
	try
	{
		int recordedNum = 0;
		if (isEvolution) // ��ȭ �����ϰ� �ٲٰ� ������ 1�� �����ϱ� ���� 1�� ����
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
// ����ó�� �Ϸ�� �Լ�
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
// �����̸� true ��ȯ 
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
			if (true == (*targetCardIter).get()->IsEvoution()) // ��ȭ�ؾ� �ϴ� ī�� �ΰ�?
			{
				return false;
			}
			int addExp = (*sourceCardIter).get()->GetCardExp();
			//const int beforeExp = (*targetCardIter).get()->GetCurExp();
			int resultExp = 0;
			// �ٷ� ������ ��ȭ�ؾ� �ϴ� ī�� ���� �ƴ��� �˻縦 �����Ƿ� SetExp�� �׻� true�� ��ȯ�ϰ� �Ǿ��ִ�.
			(*targetCardIter).get()->SetExp(addExp, resultExp);
			if (true == (*targetCardIter).get()->IsEvoution())
			{
				// ��ȭ ���� ��� �Լ� ȣ��
				if (false == SaveUserCardEvolution(true, targetCard, shared_clientInfo))
				{
					return false;
				}
			}
			// SaveUserCardEvolution�Լ����� false�� ��ȯ���� �ʾҴٸ� true�� �����Ƿ� ����ó�� �Ƚ�Ŵ
			SaveUserCardExp(resultExp, targetCard, shared_clientInfo);
			DecreaseCardAmount(sourceCard, shared_clientInfo);
			return true;
		}
	}
	return false;
}


bool CCardManager::GacharCard(const LinkPtr & shared_clientInfo)
{
	// �ϴ� �� ���� ��µ� �����ϸ�
	if (true == shared_clientInfo->PayCardGachar())
	{
		int resultCardNumber = mGacharHandler.GaCharResult(RandNumber());
		if (-1 == resultCardNumber)
		{
			return false;
		}
		if (IncreaseCardAmount(resultCardNumber, shared_clientInfo))
		{
			string resultCard("���� ī�� �̸� : " + CardStatic->GetCardName(resultCardNumber));
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


