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
	// 유저 카드 갯수 기록
	bool SaveUserCardAmount(const int& saveCardAmount, const int& userPKnum, const int& cardNum);
	// 유저 카드 경험치 기록
	bool SaveUserCardExp(const int& saveExp, const int& userPKnum, const int& cardNum);
	// 유저 카드 각성 유무 기록
	bool SaveUserCardAwake(const bool& isAwake, const int& userPKnum, const int& cardNum);
	// 유저 카드 별 기록
	bool SaveUserCardStar(const int& saveStar, const int& userPKnum, const int& cardNum);
public:
	CCardManager();
	~CCardManager();
	//const int getCardAmount(const char* textName, char* userName, int cardNum);
	// 카드 합성
	int ComposeCard(CLink& targetClient, int targetCard, int sourceCard);
	// 카드 뽑기
	int GacharCard(CLink & targetClient, char*& resultCardName);
	// 카드 각성 이름바꾸기
	int AwakeCard(CLink& targetClient, int targetCard);
};