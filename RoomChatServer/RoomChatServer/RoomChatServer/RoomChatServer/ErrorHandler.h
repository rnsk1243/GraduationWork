#pragma once
#include<vector>
#include<iostream>
#include<process.h>
using namespace std;
#include"ConstEnumInfo.h"
#include"ErrorCode.h"
#include"Link.h"
class CLink;
class CCommandController;

class CErrorHandler
{
private:
	EnumErrorCode CriticalError(EnumErrorCode code, LinkPtr client);
	EnumErrorCode TakeNullLinkError(EnumErrorCode code);
	EnumErrorCode TakeSucces(EnumErrorCode code, LinkPtr client = nullptr);
	// 에러 뜰 경우 이전 유저 카드 정보 txt에 저장 할 것.
	EnumErrorCode TakeError(EnumErrorCode code, LinkPtr client);
	CErrorHandler();
	CErrorHandler(const CErrorHandler&) = delete;
	CErrorHandler& operator=(const CErrorHandler&) = delete;
	~CErrorHandler();
	bool GetErrorMemberInfo(LinkPtr client, vector<string>& memberInfoVec);
	void GetErrorLevel(ErrorLevel level, vector<string>& errorLevelStringVec);
	void GetErrorCurTime(vector<string>& timeStringVec);
	void GetErrorCode(EnumErrorCode code, vector<string>& errorCodeStringVec);
public:
	static CErrorHandler* GetInstance();
	EnumErrorCode ErrorHandler(EnumErrorCode code, LinkPtr client = nullptr);
};
static CErrorHandler* ErrorHandStatic = CErrorHandler::GetInstance();
