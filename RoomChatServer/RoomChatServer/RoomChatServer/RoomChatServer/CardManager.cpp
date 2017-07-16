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
// ����ó�� �Ϸ�� �Լ�
MyCardVectorIt CCardManager::IsHaveCard(int cardNum, CLink& targetClient)
{
	MyCardVectorIt begin = targetClient.GetIterMyCardBegin();
	MyCardVectorIt end = targetClient.GetIterMyCardEnd();

	for (; begin != end; ++begin)
	{
		if ((*begin).get()->GetCardNumber() == cardNum)
		{
			// ī�� ��ȣ�� �ش��ϴ� �ݺ��� ����
			return begin;
		}
	}
	ErrorHandStatic->ErrorHandler(ERROR_NULL_CARD_ITERATOR, &targetClient);
	return end;
}
// ����ó�� �Ϸ�� �Լ�
bool CCardManager::IncreaseCardAmount(int cardNum, CLink& targetClient)
{
	MyCardVectorIt cardIter = IsHaveCard(cardNum, targetClient); //
	if (targetClient.GetIterMyCardEnd() != cardIter)
	{
		int increasedCardAmount = (*cardIter)->GetAmount() + 1;
		if (SaveUserCardAmount(increasedCardAmount, cardNum, &targetClient))
		{
			(*cardIter)->IncreaseCard(); // ���� �����ϸ� ���� ������Ŵ
			return true;
		}
	}
	ErrorHandStatic->ErrorHandler(ERROR_INCREACE_CARD, &targetClient);
	return false;
}
// ����ó�� �Ϸ�� �Լ�
bool CCardManager::DecreaseCardAmount(int cardNum, CLink& targetClient)
{
	MyCardVectorIt cardIter = IsHaveCard(cardNum, targetClient);
	if (targetClient.GetIterMyCardEnd() != cardIter)
	{
		
		int curCardAmount = (*cardIter)->GetAmount() - 1;
		if (curCardAmount < 0)
		{
			ErrorHandStatic->ErrorHandler(ERROR_DECREACE_CARD, &targetClient);
			return false;
		}
		if (SaveUserCardAmount(curCardAmount, cardNum, &targetClient))
		{
			(*cardIter)->DecreaseCard();
			return true;
		}
	}
	ErrorHandStatic->ErrorHandler(ERROR_DECREACE_CARD, &targetClient);
	return false;
}
// ����ó�� �Ϸ�� �Լ�
bool CCardManager::IncreaseCardStar(int cardNum, CLink & targetClient)
{
	MyCardVectorIt cardIter = IsHaveCard(cardNum, targetClient);
	if (targetClient.GetIterMyCardEnd() != cardIter)
	{
		(*cardIter)->IncreaseStar();
		int curCardStar = (*cardIter)->GetStar();
		if (SaveUserCardStar(curCardStar, cardNum, &targetClient))
		{
			return true;
		}
	}
	ErrorHandStatic->ErrorHandler(ERROR_EVOLUTION_CARD, &targetClient);
	return false;
}
// ����ó�� �Ϸ�� �Լ�
bool CCardManager::SaveUserCardAmount(const int& saveCardAmount, const int& cardNum, CLink* client)
{
	try
	{
		int moveCurserSize = -1;
		int userPKnum = client->GetMyPKNumber();
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
		ErrorHandStatic->ErrorHandler(ERROR_SAVE_CARD_AMOUNT, client);
		return false;
	}
}
// ���� ó�� �Ϸ�� �Լ�
bool CCardManager::SaveUserCardExp(const int& saveExp, const int& cardNum, CLink* client)
{
	try
	{
		int moveCurserSize = -1;
		int userPKnum = client->GetMyPKNumber();
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
		ErrorHandStatic->ErrorHandler(ERROR_SAVE_CARD_EXP, client);
		return false;
	}
}
// ����ó�� �Ϸ�� �Լ�
bool CCardManager::SaveUserCardEvolution(const bool & isEvolution, const int & cardNum, CLink* client)
{
	try
	{
		int recordedNum = 0;
		if (isEvolution) // ��ȭ �����ϰ� �ٲٰ� ������ 1�� �����ϱ� ���� 1�� ����
		{
			recordedNum = 1;
		}
		int moveCurserSize = -1;
		int userPKnum = client->GetMyPKNumber();
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
bool CCardManager::SaveUserCardStar(const int & saveStar, const int & cardNum, CLink* client)
{
	try
	{
		int moveCurserSize = -1;
		int userPKnum = client->GetMyPKNumber();
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
		ErrorHandStatic->ErrorHandler(ERROR_SAVE_CARD_STAR, client);
		return false;
	}
}
// �����̸� true ��ȯ 
bool CCardManager::ComposeCard(CLink & targetClient, int targetCard, int sourceCard)
{
	MyCardVectorIt targetCardIter = IsHaveCard(targetCard, targetClient);
	MyCardVectorIt sourceCardIter = IsHaveCard(sourceCard, targetClient);

	if (targetClient.GetIterMyCardEnd() != targetCardIter && targetClient.GetIterMyCardEnd() != sourceCardIter)
	{
		const int targetCardAmount = (*targetCardIter)->GetAmount();
		const int sourceCardAmount = (*sourceCardIter)->GetAmount();
		if ((targetCardAmount > 0) && (sourceCardAmount > 0))
		{
			if (true == (*targetCardIter).get()->IsEvoution()) // ��ȭ�ؾ� �ϴ� ī�� �ΰ�?
			{
				ErrorHandStatic->ErrorHandler(ERROR_COMPOSE_EVOUTION_CARD, &targetClient);
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
				if (false == SaveUserCardEvolution(true, targetCard, &targetClient))
				{
					return false;
				}
			}
			// SaveUserCardEvolution�Լ����� false�� ��ȯ���� �ʾҴٸ� true�� �����Ƿ� ����ó�� �Ƚ�Ŵ
			SaveUserCardExp(resultExp, targetCard, &targetClient);
			DecreaseCardAmount(sourceCard, targetClient);
			ErrorHandStatic->ErrorHandler(SUCCES_COMPOSE_CARD, &targetClient);
			return true;
		}
	}
	ErrorHandStatic->ErrorHandler(ERROR_COMPOSE_NULL_CARD, &targetClient);
	return false;
}


bool CCardManager::GacharCard(CLink & targetClient, int& resultCardNum, char*& resultCardName)
{
	// �ϴ� �� ���� ��µ� �����ϸ�
	if (true == targetClient.PayCardGachar())
	{
		int resultCardNumber = mGacharHandler.GaCharResult(RandNumber());
		if (-1 == resultCardNumber)
		{
			ErrorHandStatic->ErrorHandler(ERROR_GACHAR, &targetClient); // ���� ���̰� ī��� �� ����. // �α� Ȯ���ؼ� ���� �ʿ�
			return false;
		}
		resultCardName = CardStatic->GetCardName(resultCardNumber);
		if (IncreaseCardAmount(resultCardNumber, targetClient))
		{
			resultCardNum = resultCardNumber;
			return true;
		}
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
				ErrorHandStatic->ErrorHandler(ERROR_COMPOSE_NO_EVOUTION_CARD, &targetClient);
				return false;
			}
			const int curStar = IncreaseCardStar(targetCard, targetClient);
			SaveUserCardStar(curStar, targetCard, &targetClient);
			SaveUserCardEvolution(false, targetCard, &targetClient);
			DecreaseCardAmount(targetCard, targetClient);
			ErrorHandStatic->ErrorHandler(SUCCES_EVOLUTION_CARD, &targetClient);
			return true;
		}
		else
		{
			ErrorHandStatic->ErrorHandler(ERROR_CARD_AMOUNT_LACK, &targetClient);
			return false;
		}
	}
	ErrorHandStatic->ErrorHandler(ERROR_NULL_CARD_ITERATOR, &targetClient);
	return false;
}


