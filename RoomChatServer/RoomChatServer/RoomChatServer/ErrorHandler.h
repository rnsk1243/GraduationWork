#pragma once
#include<iostream>
#include<process.h>
using namespace std;
#include"ConstEnumInfo.h"
#include"ErrorCode.h"

class CErrorHandler
{
	static EnumErrorCode TakeError(EnumErrorCode code)
	{
		cout << "�����ڵ� = " << EnumErrorCode(code) << endl;
		//_endthreadex(0);
		return code;
	}
	static EnumErrorCode EndThread(EnumErrorCode code)
	{
		cout << "����ġ ���� ������ ���� �����ڵ� = " << EnumErrorCode(code) << endl;
		
		_endthreadex(0);
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
		case ERROR_RECV_ZERO:
		case ERROR_SHARED_COUNT_ZORO:
		case ERROR_RECV:
		case ERROR_SEND:
			return EndThread(code);
		default:
			return TakeError(code);
		}
	}
};
