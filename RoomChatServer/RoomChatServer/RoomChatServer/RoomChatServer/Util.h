#pragma once
#include<random>
#include<iostream>
using namespace std;

static bool IntToAlphabet(const int num, char* chResult)
{
	if(num < 0 || num >= 100)
	{
		CErrorHandler::ErrorHandler(ERROR_INT_TO_ALPHABET_OUT_RANGE);
		return false;
	}

	char temp[10];
	if (10 > num && 0 <= num)
	{
		chResult[0] = '0';
		_itoa_s(num, temp, 10);
		chResult[1] = temp[0];
		chResult[2] = '\0';
	}
	else if(10 <= num && 100 > num)
	{
		_itoa_s(num, temp, 10);
		chResult[0] = temp[0];
		chResult[1] = temp[1];
		chResult[2] = '\0';
	}

	return true;
}

static const string IntToString(const int& targetInt)
{
	string result;
	char tempUserNum[NameSize];
	_itoa_s(targetInt, tempUserNum, 10);
	result = tempUserNum;
	return result;
}

// 자릿수 누적
static const bool AddCipHer(const int number, int& cipHerResult)
{
	if (number < 1 || number > 1000)
	{
		CErrorHandler::ErrorHandler(ERROR_CIPHER_OUT_RANGE);
		return false;
	}
	//int result = 0;
	int curNumber = 0;
	while (curNumber != number)
	{
		curNumber++;
		if (curNumber < 10)
		{
			++cipHerResult;
		}
		else if (curNumber >= 10 && curNumber < 100)
		{
			cipHerResult += 2;
		}
		else if (curNumber >= 100 && curNumber < 1000)
		{
			cipHerResult += 3;
		}
	}
//	cipHerResult = result;

	return true;
}

static const int RandNumber(int max = 100)
{
	// 기본 1 이상 100 이하 값 나옴
	random_device rn;
	mt19937_64 rnd(rn());

	uniform_int_distribution<int> range(1, max);
	return range(rnd);
}