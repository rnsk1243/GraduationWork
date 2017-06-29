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

static const string IntToString(const int& targetInt)
{
	string result;
	char tempUserNum[NameSize];
	_itoa_s(targetInt, tempUserNum, 10);
	result = tempUserNum;
	return result;
}

// ÀÚ¸´¼ö ´©Àû
static const int AddCipHer(const int Number)
{
	if (Number < 1)
		return -1;
	int result = 0;
	int curNumber = 0;
	int objLine = Number;
	while (curNumber != objLine)
	{
		curNumber++;
		if (curNumber < 10)
		{
			++result;
		}
		else if (curNumber >= 10 && curNumber < 100)
		{
			result += 2;
		}
		else if (curNumber >= 100 && curNumber < 1000)
		{
			result += 3;
		}
	}
	return result;
}

