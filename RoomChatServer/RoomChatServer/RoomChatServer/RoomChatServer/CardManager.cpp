#include "CardManager.h"
#include"WriteHandler.h"
#include"ReadHandler.h"
#include"Util.h"
#include"ErrorHandler.h"
#include"ErrorCode.h"
#include"ConstEnumInfo.h"
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
//	int line = ReadHandlerStatic->Search(textName, false, 1, strUserName);
//	WriteHandlerStatic->BeginToTargetUserLineCurserMoveSize(textName, line, targetLine);
//	IntToAlphabet(cardNum, chCardNum);
//	string strCardNum = chCardNum;
//	WriteHandlerStatic->TargetLineToUserCurserMoveSize(targetLine, strCardNum, isNewCard, cardAmount);
//	return cardAmount;
//}

bool CCardManager::ChangeUserCardAmount(const char* textName, char* userName, int cardNum, bool isIncrease)
{
	try
	{
		char targetLine[BufSize];
		char chCardNum[3];
		bool isNewCard = true;
		int cardAmount = 0;
		string strUserName = userName;
		int line = ReadHandlerStatic->Search(textName, false, 1, strUserName);
		int moveSize1 = WriteHandlerStatic->BeginToTargetUserLineCurserMoveSize(textName, line, targetLine);
		IntToAlphabet(cardNum, chCardNum);
		string strCardNum = chCardNum;
		int movesize2 = WriteHandlerStatic->TargetLineToUserCurserMoveSize(targetLine, strCardNum, isNewCard, cardAmount);
		if (0 > cardAmount && isIncrease == false)
		{
			CErrorHandler::ErrorHandler(ERROR_DECREASE_CARD);
			return false;
		}
		int moveCurserSize = moveSize1 + movesize2;
		isIncrease ? cardAmount++ : cardAmount--;
		WriteHandlerStatic->WriteCard(textName, moveCurserSize, cardNum, cardAmount, isNewCard);
		return true;
	}
	catch (const std::exception&)
	{
		isIncrease ? CErrorHandler::ErrorHandler(ERROR_INCREASE_CARD) : CErrorHandler::ErrorHandler(ERROR_DECREASE_CARD);
		return false;
	}
}

