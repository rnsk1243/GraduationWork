#pragma once
class CCardManager
{
public:
	CCardManager();
	~CCardManager();
	//const int getCardAmount(const char* textName, char* userName, int cardNum);
	// ���� ī�� ���� ���� (����txt�̸�, �����̸�, ī���ȣ, ����or����)
	bool ChangeUserCardAmount(const char * textName, char * userName, int cardNum, bool isIncrease);

};

