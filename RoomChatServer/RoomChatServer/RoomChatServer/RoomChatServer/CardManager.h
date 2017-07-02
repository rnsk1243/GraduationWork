#pragma once
#include"GaChar.h"
#include"ConstEnumInfo.h"
#include"Link.h"

class CCardManager
{
private:
	CGaChar mGacharHandler;
	int GetMoveCurserSize(const int& userPK, WhatCardCurserSize whatCardInfoCurser, const int & cardNum);
	int IsHaveCard(int cardNum, MyCardVectorIt& cardIter, CLink& targetClient);
	int IncreaseCardAmount(int cardNum, CLink& targetClient);
	int DecreaseCardAmount(int cardNum, CLink& targetClient);
	int IncreaseCardStar(int cardNum, CLink& targetClient);
	// ���� ī�� ���� ���
	bool SaveUserCardAmount(const int& saveCardAmount, const int& userPKnum, const int& cardNum);
	// ���� ī�� ����ġ ���
	bool SaveUserCardExp(const int& saveExp, const int& userPKnum, const int& cardNum);
	// ���� ī�� ��ȭ ���� ���
	bool SaveUserCardEvolution(const bool& isEvolution, const int& userPKnum, const int& cardNum);
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
	// ī�� ��ȭ
	int EvolutionCard(CLink& targetClient, int targetCard);
};