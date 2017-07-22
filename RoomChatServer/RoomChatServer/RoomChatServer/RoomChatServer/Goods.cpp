#include "Goods.h"
#include"ErrorHandler.h"
#include"Util.h"
#include"WriteHandler.h"
#include"Link.h"

CGoods::CGoods(const int& pkNumber):
	mClientPK(pkNumber), 
	mGoods(0)
{
	cout << "CGOODS 생성자 = " << pkNumber << endl;
}


CGoods::~CGoods()
{
}

bool CGoods::GetMoveCurserSizeGoods(WhatGoodsCursorSize whatCardInfoCurser, int& resultCursorSize)
{
	//int tempResult = 0;
	int moveSize1 = 0;
	cout << "mClientPK = " << mClientPK << endl;
	if (false == AddCipHer(mClientPK, moveSize1))// 회원 번호까지 누적 자릿수 더하기
	{
		ErrorHandStatic->ErrorHandler(ERROR_CURSER_SIZE);
		return false;
	}
	const int userPKnumBefore = mClientPK - 1;
	// 회원 번호까지 재화 템플릿 누적 마지막에 1더한 것은 pk 옆 | 때문임.
	const int moveSize2 = (userPKnumBefore * GoodsTemplateSize) + 1;
	const int moveCurserSize = moveSize1 + moveSize2;
	//tempResult = moveCurserSize;
	resultCursorSize = moveCurserSize;
	switch (whatCardInfoCurser)
	{
	case WhatGoodsCursorSize::Money:
		resultCursorSize += 0;
		break;
	default:
		return false;
	}
	//resultCursorSize = tempResult;
	return true;
}

bool CGoods::SaveUserMoney(const int & saveMoney, EnumErrorCode& resultcode)
{
	ScopeLock<MUTEX> MU(mRAII_GoodsMUTEX);
	try
	{
		int moveCursorSize = -1;
		if (true == GetMoveCurserSizeGoods(WhatGoodsCursorSize::Money, moveCursorSize))
		{
			if (true == WriteHandlerStatic->WriteObj(NameMemberGoodsTxt, moveCursorSize, saveMoney, MaxMoneyCipher))
			{
				return true;
			}
		}
		resultcode = ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY);
		return false;
	}
	catch (const std::exception&)
	{
		resultcode = ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY);
		return false;
	}
}

bool CGoods::IsZeroMoney()
{
	if (MinMoney == mGoods.money)
	{
		return true;
	}
	return false;
}

bool CGoods::SetZeroMoney(EnumErrorCode& resultcode)
{
	if (MinusMyMoney(GetMyMoney(), resultcode))
	{
		return true;
	}
	return false;
}

bool CGoods::InitMoney(int & initMoney)
{
	if (initMoney >= 0 && initMoney <= MaxMoney)
	{
		mGoods.money = initMoney;
		return true;
	}
	else
	{
		return false;
	}

}

bool CGoods::AddMyMoney (const int& addMoney, EnumErrorCode& resultcode)
{
	int saveMoney = (GetMyMoney() + addMoney);
	if (MaxMoney < saveMoney)
	{
		resultcode = ErrorHandStatic->ErrorHandler(ERROR_ADD_MONEY_MAX);
		return false;
	}
	if (true == SaveUserMoney(saveMoney, resultcode))
	{
		mGoods.money += addMoney;
		return true;
	}
	return false;
}

bool CGoods::MinusMyMoney(int minusMoney, EnumErrorCode& resultcode)
{
	int saveMoney = (GetMyMoney() - minusMoney);
	if (MinMoney > saveMoney)
	{
		resultcode = ErrorHandStatic->ErrorHandler(ERROR_MINUS_MONEY_MIN);
		return false;
	}

	if (true == SaveUserMoney(saveMoney, resultcode)) // txt파일에 신규 돈 저장 하면
	{
		mGoods.money -= minusMoney; // 자료 돈 저장
		return true;
	}
	return false;
}

