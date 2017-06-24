#pragma once
#include<iostream>
#include<list>
#include<memory>
using namespace std;
#include"Card.h"

class CGaChar
{
public:
	CGaChar();
	CGaChar(const CGaChar& copy) = delete;
	CGaChar& operator=(const CGaChar& copy) = delete;
	~CGaChar();
	int randNumber(int max = 100);// 기본 1 이상 100 이하 값 나옴
	Card* gaCharResult(int range);
};
