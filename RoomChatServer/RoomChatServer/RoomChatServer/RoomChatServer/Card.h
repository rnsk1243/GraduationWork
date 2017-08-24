#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include"ConstEnumInfo.h"
using namespace std;

struct Card
{
	const int mCardNum;
	string mName;
	const int mProb; // Ȯ��
	const int mStat; // ����
	const  int mGiveExp; // �� ī�带 �ռ��ϸ� ��� ����ġ
	Card(int num, char* cardName, int prob, int stat, int giveExp) :
		mCardNum(num),
		mProb(prob),
		mStat(stat),
		//name(cardName),
		mGiveExp(giveExp),
		mName(cardName)
	{
	}

	~Card() {
		cout << "ī�� �Ҹ�" << endl;
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

typedef shared_ptr<Card> CardPtr;
typedef vector<CardPtr> CardVector;
typedef CardVector::const_iterator CardConstVectorIt; // const_iterator �ٿ��� ����Ű�� ��ü ���� �Ұ�

class CCard
{
	CardVector mCards;
	CCard();
	CardConstVectorIt GetCardListIterBegin() { return mCards.begin(); }
	CardConstVectorIt GetCardListIterEnd() { return mCards.end(); }
	Card* GetCard(int cardNum);
public:
	CCard(const CCard& copy) = delete;
	CCard& operator=(const CCard& copy) = delete;
	~CCard();
	static CCard* GetInstance();
	void PushCard(const CardPtr& card);
	int GetCardStat(int cardNum);
	int GetCardProb(int cardNum);
	string GetCardName(int cardNum);
	int GetCardExp(int cardNum);
};
static CCard* CardStatic = CCard::GetInstance();
