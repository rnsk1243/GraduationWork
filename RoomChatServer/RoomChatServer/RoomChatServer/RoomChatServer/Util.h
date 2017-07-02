#pragma once
#include<random>
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

// 자릿수 누적
static const int AddCipHer(const int number)
{
	if (number < 1)
		return -1;
	int result = 0;
	int curNumber = 0;
	while (curNumber != number)
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

static const int RandNumber(int max = 100)
{
	// 기본 1 이상 100 이하 값 나옴
	random_device rn;
	mt19937_64 rnd(rn());

	uniform_int_distribution<int> range(1, max);
	return range(rnd);
}