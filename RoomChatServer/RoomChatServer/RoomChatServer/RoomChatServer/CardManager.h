#pragma once
#include"ErrorHandler.h"
#include"GaChar.h"
#include"ConstEnumInfo.h"
#include"Link.h"

class CCardManager
{
private:
	CGaChar mGacharHandler;
	bool GetMoveCurserSize(const int& userPK, WhatCardCurserSize whatCardInfoCurser, const int & cardNum, int& resultCursorSize);
	MyCardVectorIt IsHaveCard(int cardNum, CLink& targetClient);
	bool IncreaseCardAmount(int cardNum, CLink& targetClient); // ī�� ���� ����
	bool DecreaseCardAmount(int cardNum, CLink& targetClient); // ī�� ���� ����
	bool IncreaseCardStar(int cardNum, CLink& targetClient); // �� ����
	// ���� ī�� ���� ���
	bool SaveUserCardAmount(const int& saveCardAmount, const int& userPKnum, const int& cardNum, CLink* targetClient);
	// ���� ī�� ����ġ ���
	bool SaveUserCardExp(const int& saveExp, const int& userPKnum, const int& cardNum, CLink* client);
	// ���� ī�� ��ȭ ���� ���
	bool SaveUserCardEvolution(const bool& isEvolution, const int& userPKnum, const int& cardNum, CLink* client);
	// ���� ī�� �� ���
	bool SaveUserCardStar(const int& saveStar, const int& userPKnum, const int& cardNum, CLink* client);
public:
	CCardManager();
	~CCardManager();
	//const int getCardAmount(const char* textName, char* userName, int cardNum);
	// ī�� �ռ�
	bool ComposeCard(CLink& targetClient, int targetCard, int sourceCard);
	// ī�� �̱�
	bool GacharCard(CLink & targetClient, int& resultCardNum, char*& resultCardName);
	// ī�� ��ȭ
	bool EvolutionCard(CLink& targetClient, int targetCard);
};