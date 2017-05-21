#pragma once
#include<iostream>
#include<list>
#include<memory>
using namespace std;
const int BufferSize = 256;

struct Card
{
	int cardNum;
	char* name;
	int prob; // Ȯ��
	int stat; // ����
	Card(int num, char* cardName, int prob_, int stat_):
		cardNum(num),
		prob(prob_),
		stat(stat_),
		name(cardName)
		//name(new char[BufferSize])
	{
		//int size = strlen(cardName) + 1;
		//strcpy_s(name, size, cardName);
		//name[size] = '\0';
	}

	~Card(){
		cout << "ī�� �Ҹ�" << endl;
		delete[] name;
	}
	Card(const Card& copy) = delete;
	Card& operator=(const Card& copy) = delete;
};



class CGaChar
{
	list<shared_ptr<Card>> mCards;
//	list<shared_ptr<Card>>* getCards() { return &mCards; }
	CGaChar();
public:
	~CGaChar();
	static CGaChar* getInstance();
	int randNumber(int max = 100);// �⺻ 1 �̻� 100 ���� �� ����
	Card* gaCharResult(int range);
	void pushCard(shared_ptr<Card> card);
};
static CGaChar* GaCharStatic = CGaChar::getInstance();
