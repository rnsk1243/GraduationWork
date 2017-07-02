#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include"ConstEnumInfo.h"
using namespace std;

struct Card
{
	int cardNum;
	char* name;
	int prob; // Ȯ��
	int stat; // ����
	int giveExp; // �� ī�带 �ռ��ϸ� ��� ����ġ
	Card(int num, char* cardName, int prob_, int stat_, int giveExp_) :
		cardNum(num),
		prob(prob_),
		stat(stat_),
		//name(cardName),
		giveExp(giveExp_),
		name(new char[CardNameBuf])
	{
		size_t size = strlen(cardName) + 1;
		strcpy_s(name, size, cardName);
		name[size] = '\0';
	}

	~Card() {
		cout << "ī�� �Ҹ�" << endl;
		delete[] name;
	}
	Card(const Card& copy) = delete;
	//Card(const Card& copy):
	//	cardNum(copy.cardNum),
	//	prob(copy.prob),
	//	stat(copy.stat)
	//{
	//	cout << "ī�� ���� ������ ȣ��" << endl;
	//	size_t nameSize = strlen(copy.name) + 1;
	//	name = new char[nameSize];
	//	strcpy_s(name, nameSize, copy.name);
	//}
	Card& operator=(const Card& copy) = delete;
};

typedef vector<shared_ptr<Card>> CardVector;
typedef CardVector::iterator CardVectorIt;

class CCard
{
	CardVector mCards;
	CCard();
public:
	CCard(const CCard& copy) = delete;
	CCard& operator=(const CCard& copy) = delete;
	~CCard();
	static CCard* getInstance();
	void pushCard(shared_ptr<Card> card);
	CardVectorIt getCardListIterBegin() { return mCards.begin(); }
	CardVectorIt getCardListIterEnd() { return mCards.end(); }
};
static CCard* CardStatic = CCard::getInstance();
