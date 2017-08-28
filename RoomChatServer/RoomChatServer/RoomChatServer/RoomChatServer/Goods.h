#pragma once
#include"RAII.h"
#include"ConstEnumInfo.h"
#include"ErrorCode.h"
class CLink;

struct Goods
{
	int money;
	Goods(int _money)
	{
		money = _money;
	}
	Goods(const Goods& copy) = delete;
	Goods& operator=(const Goods& copy) = delete;
};

class CGoods
{
	const int mClientPK; // 이 재화의 주인 고유 번호
	Goods mGoods;
	MUTEX mRAII_GoodsMUTEX;
	bool GetMoveCurserSizeGoods(WhatGoodsCursorSize whatCardInfoCurser, int& resultCursorSize);
	bool SaveUserMoney(const int & saveMoney);
public:
	CGoods(const CGoods& copy) = delete;
	CGoods& operator=(const CGoods& copy) = delete;
	CGoods(const int& pkNumber);
	~CGoods();
	bool IsZeroMoney();
	void SetZeroMoney();
	int GetMyMoney() { return mGoods.money; }
	bool InitMoney(int& initMoney);
	bool AddMyMoney(const int& addMoney);
	void MinusMyMoney(int minusMoney);
};

