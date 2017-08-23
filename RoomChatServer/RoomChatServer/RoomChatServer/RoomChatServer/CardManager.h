#pragma once
#include"ErrorHandler.h"
#include"GaChar.h"
#include"ConstEnumInfo.h"
#include"Link.h"

class CCardManager
{
private:
	CGaChar mGacharHandler;
	bool GetMoveCurserSize(const int& userPK, WhatCardCursorSize whatCardInfoCurser, const int & cardNum, int& resultCursorSize);
	MyCardVectorIt GetMyCard(int cardNum, const LinkPtr & shared_clientInfo);
	bool IncreaseCardAmount(int cardNum, const LinkPtr & shared_clientInfo); // ī�� ���� ����
	bool DecreaseCardAmount(int cardNum, const LinkPtr & shared_clientInfo); // ī�� ���� ����
	bool IncreaseCardStar(int cardNum, const LinkPtr & shared_clientInfo); // �� ����
	// ���� ī�� ���� ���
	bool SaveUserCardAmount(const int& saveCardAmount, const int& cardNum, const LinkPtr & shared_clientInfo);
	// ���� ī�� ����ġ ���
	bool SaveUserCardExp(const int& saveExp, const int& cardNum, const LinkPtr & shared_clientInfo);
	// ���� ī�� ��ȭ ���� ���
	bool SaveUserCardEvolution(const bool& isEvolution, const int& cardNum, const LinkPtr & shared_clientInfo);
	// ���� ī�� �� ���
	bool SaveUserCardStar(const int& saveStar, const int& cardNum, const LinkPtr & shared_clientInfo);
public:
	CCardManager();
	~CCardManager();
	//const int getCardAmount(const char* textName, char* userName, int cardNum);
	// ī�� �ռ�
	bool ComposeCard(const LinkPtr & shared_clientInfo, int targetCard, int sourceCard);
	// ī�� �̱�
	bool GacharCard(const LinkPtr & shared_clientInfo);
	// ī�� ��ȭ
	bool EvolutionCard(const LinkPtr & shared_clientInfo, int targetCard);
};