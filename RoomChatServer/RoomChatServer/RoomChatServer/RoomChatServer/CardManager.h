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
	bool IncreaseCardAmount(int cardNum, CLink& targetClient); // 카드 갯수 증가
	bool DecreaseCardAmount(int cardNum, CLink& targetClient); // 카드 갯수 감소
	bool IncreaseCardStar(int cardNum, CLink& targetClient); // 별 증가
	// 유저 카드 갯수 기록
	bool SaveUserCardAmount(const int& saveCardAmount, const int& userPKnum, const int& cardNum, CLink* targetClient);
	// 유저 카드 경험치 기록
	bool SaveUserCardExp(const int& saveExp, const int& userPKnum, const int& cardNum, CLink* client);
	// 유저 카드 진화 유무 기록
	bool SaveUserCardEvolution(const bool& isEvolution, const int& userPKnum, const int& cardNum, CLink* client);
	// 유저 카드 별 기록
	bool SaveUserCardStar(const int& saveStar, const int& userPKnum, const int& cardNum, CLink* client);
public:
	CCardManager();
	~CCardManager();
	//const int getCardAmount(const char* textName, char* userName, int cardNum);
	// 카드 합성
	bool ComposeCard(CLink& targetClient, int targetCard, int sourceCard);
	// 카드 뽑기
	bool GacharCard(CLink & targetClient, int& resultCardNum, char*& resultCardName);
	// 카드 진화
	bool EvolutionCard(CLink& targetClient, int targetCard);
};