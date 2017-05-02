#pragma once
#include<iostream>
#include<process.h>
using namespace std;
#include"ConstEnumInfo.h"
#include"ErrorCode.h"

class CErrorHandler
{
	static EnumErrorCode EndClientThread(EnumErrorCode code)
	{
		cout << "에러코드 = " << EnumErrorCode(code) << endl;
		//_endthreadex(0);
		return code;
	}
	CErrorHandler();
	CErrorHandler(const CErrorHandler&);
	CErrorHandler& operator=(const CErrorHandler&);
	~CErrorHandler();
public:
	static EnumErrorCode ErrorHandler(EnumErrorCode code)
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
