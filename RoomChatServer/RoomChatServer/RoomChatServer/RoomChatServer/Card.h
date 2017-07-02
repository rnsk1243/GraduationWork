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
	int prob; // 확률
	int stat; // 스탯
	int giveExp; // 이 카드를 합성하면 얻는 경험치
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
		cout << "카드 소멸" << endl;
		delete[] name;
	}
	Card(const Card& copy) = delete;
	//Card(const Card& copy):
	//	cardNum(copy.cardNum),
	//	prob(copy.prob),
	//	stat(copy.stat)
	//{
	//	cout << "카드 복사 생성자 호출" << endl;
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
