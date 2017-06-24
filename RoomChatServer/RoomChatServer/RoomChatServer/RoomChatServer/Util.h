#pragma once
#include<iostream>
using namespace std;

static void IntToAlphabet(const int num, char* chResult)
{
	char temp[10];
	if (10 > num)
	{
		chResult[0] = '0';
		_itoa_s(num, temp, 10);
		chResult[1] = temp[0];
		chResult[2] = '\0';
	}
	else
	{
		_itoa_s(num, temp, 10);
		chResult[0] = temp[0];
		chResult[1] = temp[1];
		chResult[2] = '\0';
	}
}