#pragma once
#include"RAII.h"
#include"ConstEnumInfo.h"
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
	Goods mGoods;
	MUTEX mRAII_GoodsMUTEX;
	bool GetMoveCurserSizeGoods(const int& userPK, WhatGoodsCursorSize whatCardInfoCurser, int& resultCursorSize);
	bool SaveUserMoney(const int & saveMoney, CLink * client);
public:
	CGoods(const CGoods& copy) = delete;
	CGoods& operator=(const CGoods& copy) = delete;
	CGoods();
	~CGoods();
	bool IsZeroMoney();
	bool SetZeroMoney(CLink* client);
	int GetMyMoney() { return mGoods.money; }
	bool InitMoney(int& initMoney);
	bool AddMyMoney(const int& addMoney, CLink* client);
	bool MinusMyMoney(int minusMoney, CLink* client);
};

