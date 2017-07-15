#include "Goods.h"
#include"ErrorHandler.h"
#include"Util.h"
#include"WriteHandler.h"
#include"Link.h"

CGoods::CGoods():mGoods(0)
{
}


CGoods::~CGoods()
{
}

bool CGoods::GetMoveCurserSizeGoods(const int& userPK, WhatGoodsCursorSize whatCardInfoCurser, int& resultCursorSize)
{
	//int tempResult = 0;
	int moveSize1 = 0;
	if (false == AddCipHer(userPK, moveSize1))// 회원 번호까지 누적 자릿수 더하기
	{
		ErrorHandStatic->ErrorHandler(ERROR_CURSER_SIZE);
		return false;
	}
	const int userPKnumBefore = userPK - 1;
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

bool CGoods::SaveUserMoney(const int& saveMoney, CLink* client)
{
	try
	{
		int moveCursorSize = -1;
		int userPK = client->GetMyPKNumber();
		if (true == GetMoveCurserSizeGoods(userPK, WhatGoodsCursorSize::Money, moveCursorSize))
		{
			if (true == WriteHandlerStatic->WriteObj(NameMemberGoodsTxt, moveCursorSize, saveMoney, MaxMoneyCipher))
			{
				return true;
			}
		}
		return false;
	}
	catch (const std::exception&)
	{
		ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY, client);
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

bool CGoods::SetZeroMoney(CLink* client)
{
	if (MinusMyMoney(GetMyMoney(), client))
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

bool CGoods::AddMyMoney (const int& addMoney, CLink* client)
{
	int saveMoney = (GetMyMoney() + addMoney);
	if (MaxMoney < saveMoney)
	{
		ErrorHandStatic->ErrorHandler(ERROR_ADD_MONEY_MAX, client);
		return false;
	}
	ScopeLock<MUTEX> MU(mRAII_GoodsMUTEX);
	if (SaveUserMoney(saveMoney, client))
	{
		mGoods.money += addMoney;
		return true;
	}
	return false;
}

bool CGoods::MinusMyMoney(int minusMoney, CLink* client)
{
	int saveMoney = (GetMyMoney() - minusMoney);
	if (MinMoney > saveMoney)
	{
		//ErrorHandStatic->ErrorHandler(ERROR_MINUS_MONEY_MIN, client);
		//return false;
		saveMoney = 0;
		minusMoney = GetMyMoney();
	}
	ScopeLock<MUTEX> MU(mRAII_GoodsMUTEX);
	if (SaveUserMoney(saveMoney, client)) // txt파일에 신규 돈 저장 하면
	{
		mGoods.money -= minusMoney; // 자료 돈 저장
		return true;
	}
	return false;
}

