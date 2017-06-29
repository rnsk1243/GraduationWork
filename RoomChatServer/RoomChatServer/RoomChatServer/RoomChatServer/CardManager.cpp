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
//	int line = ReadHandlerStatic->Search(textName, 1, strUserName);
//	WriteHandlerStatic->BeginToTargetUserLineCurserMoveSize(textName, line, targetLine);
//	IntToAlphabet(cardNum, chCardNum);
//	string strCardNum = chCardNum;
//	WriteHandlerStatic->TargetLineToUserCurserMoveSize(targetLine, strCardNum, isNewCard, cardAmount);
//	return cardAmount;
//}

bool CCardManager::ChangeUserCardAmount(const char* textName, const int& curCardAmount, const int& userPKnum, const int& cardNum)
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
		const int cardAmount = curCardAmount;
		cout << "cardAmount = " << cardAmount << "// cardNum = " << cardNum << endl;
		const int moveSize1 = AddCipHer(userPKnum); // 회원 번호까지 누적 자릿수 더하기
		const int userPKnumBefore = userPKnum - 1;
		const int moveSize2 = (userPKnumBefore * CardTemplateSize) + ((cardNum - 1) * CardInfoSize) + 1; // 회원 번호까지 카드 템플릿 누적
		const int moveCurserSize = moveSize1 + moveSize2;
		WriteHandlerStatic->WriteCard(textName, moveCurserSize, cardNum, cardAmount);
		return true;
	}
	catch (const std::exception&)
	{
		CErrorHandler::ErrorHandler(ERROR_SAVE_CARD);
		return false;
	}
}

