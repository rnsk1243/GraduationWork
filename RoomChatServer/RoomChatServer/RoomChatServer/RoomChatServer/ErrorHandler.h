#pragma once
#include<iostream>
#include<process.h>
using namespace std;
//#include"ConstEnumInfo.h"
#include"ErrorCode.h"
class CLink;
class CCommandController;

class CErrorHandler
{
private:
	CCommandController* mCommandPtr;
	EnumErrorCode CriticalError(EnumErrorCode code, CLink* client);
	EnumErrorCode TakeNullLinkError(EnumErrorCode code);
	EnumErrorCode TakeSucces(EnumErrorCode code);
	// ���� �� ��� ���� ���� ī�� ���� txt�� ���� �� ��.
	EnumErrorCode TakeError(EnumErrorCode code, CLink* client);
	CErrorHandler();
	CErrorHandler(const CErrorHandler&) = delete;
	CErrorHandler& operator=(const CErrorHandler&) = delete;
	~CErrorHandler();
public:
	static CErrorHandler* GetInstance();
	void setCommand(CCommandController* command);
	EnumErrorCode ErrorHandler(EnumErrorCode code, CLink* client = nullptr);
};
static CErrorHandler* ErrorHandStatic = CErrorHandler::GetInstance();
