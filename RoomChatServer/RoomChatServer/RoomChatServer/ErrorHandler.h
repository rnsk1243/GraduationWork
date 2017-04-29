#pragma once
#include<iostream>
#include<process.h>
using namespace std;
#include"ConstEnumInfo.h"

class CErrorHandler
{
	static int EndClientThread(const int code)
	{
		cout << "에러코드 = " << code << endl;
		//_endthreadex(0);
		return code;
	}
	CErrorHandler();
	CErrorHandler(const CErrorHandler&);
	CErrorHandler& operator=(const CErrorHandler&);
	~CErrorHandler();
public:
	static int ErrorHandler(const int code)
	{
		switch (code)
		{
		case ERROR_RECV:
		case ERROR_SEND:
			return EndClientThread(code);
		default:
			return code;
		}
	}
};
