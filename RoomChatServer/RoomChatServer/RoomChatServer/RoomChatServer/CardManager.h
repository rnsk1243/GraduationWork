#pragma once
#include"GaChar.h"
#include"ConstEnumInfo.h"
#include"Link.h"

class CCardManager
{
private:
	CGaChar mGacharHandler;
	int GetMoveCurserSize(const int& userPK, WhatCardCurserSize whatCardInfoCurser, const int & cardNum);
	int isHaveCard(int cardNum, MyCardVectorIt& cardIter, CLink& targetClient);
	int increaseCardAmount(int cardNum, CLink& targetClient);
	int decreaseCardAmount(int cardNum, CLink& targetClient);
	int increaseCardStar(int cardNum, CLink& targetClient);
	// ���� ī�� ���� ���
	bool SaveUserCardAmount(const int& saveCardAmount, const int& userPKnum, const int& cardNum);
	// ���� ī�� ����ġ ���
	bool SaveUserCardExp(const int& saveExp, const int& userPKnum, const int& cardNum);
	// ���� ī�� ���� ���� ���
	bool SaveUserCardAwake(const bool& isAwake, const int& userPKnum, const int& cardNum);
	// ���� ī�� �� ���
	bool SaveUserCardStar(const int& saveStar, const int& userPKnum, const int& cardNum);
public:
	CCardManager();
	~CCardManager();
	//const int getCardAmount(const char* textName, char* userName, int cardNum);
	// ī�� �ռ�
	int ComposeCard(CLink& targetClient, int targetCard, int sourceCard);
	// ī�� �̱�
	int GacharCard(CLink & targetClient, char*& resultCardName);
	// ī�� ���� �̸��ٲٱ�
	int AwakeCard(CLink& targetClient, int targetCard);
};