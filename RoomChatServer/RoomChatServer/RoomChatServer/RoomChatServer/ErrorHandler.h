#pragma once
#include<vector>
#include<iostream>
#include<process.h>
using namespace std;
#include"ConstEnumInfo.h"
#include"ErrorCode.h"
class CLink;
class CCommandController;

class CErrorHandler
{
private:
	//vector<string> mTimeUnit;
	CCommandController* mCommandPtr;
	EnumErrorCode CriticalError(EnumErrorCode code, CLink* client);
	EnumErrorCode TakeNullLinkError(EnumErrorCode code);
	EnumErrorCode TakeSucces(EnumErrorCode code, CLink* client = nullptr);
	// 에러 뜰 경우 이전 유저 카드 정보 txt에 저장 할 것.
	EnumErrorCode TakeError(EnumErrorCode code, CLink* client);
	CErrorHandler();
	CErrorHandler(const CErrorHandler&) = delete;
	CErrorHandler& operator=(const CErrorHandler&) = delete;
	~CErrorHandler();
	bool GetErrorMemberInfo(CLink * client, vector<string>& memberInfoVec);
	void GetErrorLevel(ErrorLevel level, vector<string>& errorLevelStringVec);
	void GetErrorCurTime(vector<string>& timeStringVec);
	void GetErrorCode(EnumErrorCode code, vector<string>& errorCodeStringVec);
public:
	static CErrorHandler* GetInstance();
	void setCommand(CCommandController* command);
	EnumErrorCode ErrorHandler(EnumErrorCode code, CLink* client = nullptr);
};
static CErrorHandler* ErrorHandStatic = CErrorHandler::GetInstance();
