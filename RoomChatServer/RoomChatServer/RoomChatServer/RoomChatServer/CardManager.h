#pragma once
class CCardManager
{
public:
	CCardManager();
	~CCardManager();
	//const int getCardAmount(const char* textName, char* userName, int cardNum);
	// ���� ī�� ���� ���� (����txt�̸�, ���簡���� �ִ� ī�� ����, ������ȣ, ī���ȣ, ����or����)
	bool ChangeUserCardAmount(const char* textName, const int& curCardAmount, const int& userPKnum, const int& cardNum);
};