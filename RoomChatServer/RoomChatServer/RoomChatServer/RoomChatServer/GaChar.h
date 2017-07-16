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
	int GaCharResult(int range);
};
