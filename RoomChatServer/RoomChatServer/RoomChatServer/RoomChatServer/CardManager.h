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
	bool IncreaseCardAmount(int cardNum, const LinkPtr & shared_clientInfo); // 카드 갯수 증가
	bool DecreaseCardAmount(int cardNum, const LinkPtr & shared_clientInfo); // 카드 갯수 감소
	bool IncreaseCardStar(int cardNum, const LinkPtr & shared_clientInfo); // 별 증가
	// 유저 카드 갯수 기록
	bool SaveUserCardAmount(const int& saveCardAmount, const int& cardNum, const LinkPtr & shared_clientInfo);
	// 유저 카드 경험치 기록
	bool SaveUserCardExp(const int& saveExp, const int& cardNum, const LinkPtr & shared_clientInfo);
	// 유저 카드 진화 유무 기록
	bool SaveUserCardEvolution(const bool& isEvolution, const int& cardNum, const LinkPtr & shared_clientInfo);
	// 유저 카드 별 기록
	bool SaveUserCardStar(const int& saveStar, const int& cardNum, const LinkPtr & shared_clientInfo);
public:
	CCardManager();
	~CCardManager();
	//const int getCardAmount(const char* textName, char* userName, int cardNum);
	// 카드 합성
	bool ComposeCard(const LinkPtr & shared_clientInfo, int targetCard, int sourceCard);
	// 카드 뽑기
	bool GacharCard(const LinkPtr & shared_clientInfo);
	// 카드 진화
	bool EvolutionCard(const LinkPtr & shared_clientInfo, int targetCard);
};