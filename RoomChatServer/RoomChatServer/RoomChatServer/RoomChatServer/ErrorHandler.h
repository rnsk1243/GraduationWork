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
		cout << "[심각한 에러 발생] 에러코드 = [ " << EnumErrorCode(code) << " ] 스레드를 강제 종료 시킵니다." << endl;
		_endthreadex(0);
		return code;
	}
	static EnumErrorCode TakeError(EnumErrorCode code)
	{
		cout << "에러코드 = " << EnumErrorCode(code) << endl;
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
