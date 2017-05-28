#pragma once
#include<iostream>
#include<process.h>
using namespace std;
#include"ConstEnumInfo.h"
#include"ErrorCode.h"

class CErrorHandler
{
	static EnumErrorCode CriticalError(EnumErrorCode code)
	{
		cout << "[�ɰ��� ���� �߻�] �����ڵ� = [ " << EnumErrorCode(code) << " ] �����带 ���� ���� ��ŵ�ϴ�." << endl;
		_endthreadex(0);
		return code;
	}
	static EnumErrorCode TakeError(EnumErrorCode code)
	{
		cout << "�����ڵ� = " << EnumErrorCode(code) << endl;
		//_endthreadex(0);
		return code;
	}
	CErrorHandler();
	CErrorHandler(const CErrorHandler&) = delete;
	CErrorHandler& operator=(const CErrorHandler&) = delete;
	~CErrorHandler();
public:
	static EnumErrorCode ErrorHandler(EnumErrorCode code)
	{
		switch (code)
		{
		case ERROR_SHARED_COUNT_ZORO:
		case ERROR_RECV:
		case ERROR_SEND:
			return CriticalError(code);
		default:
			return TakeError(code);
		}
	}
};
