#pragma once
class CCardManager
{
public:
	CCardManager();
	~CCardManager();
	//const int getCardAmount(const char* textName, char* userName, int cardNum);
	// 유저 카드 갯수 변경 (저장txt이름, 현재가지고 있는 카드 갯수, 유저번호, 카드번호, 증가or감소)
	bool ChangeUserCardAmount(const char* textName, const int& curCardAmount, const int& userPKnum, const int& cardNum);
};