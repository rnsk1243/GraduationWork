#include "ErrorHandler.h"
#include"Link.h"
#include"CommandController.h"
#include"WriteHandler.h"


EnumErrorCode CErrorHandler::CriticalError(EnumErrorCode code, CLink * client)
{
	//cout << "[심각한 에러 발생] 에러코드 = [ " << EnumErrorCode(code) << " ]" << endl;
	vector<string> errorMessageVec;
	GetErrorLevel(ErrorLevel::Serious, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	GetErrorMemberInfo(client, errorMessageVec);
	if (nullptr == client)
	{
		cout << "client가 초기화 되지 않아 심각한 오류 에러코드 = [ " << EnumErrorCode(code) << " ]를 처리할 수 없습니다." << endl;
	}else if (nullptr == mCommandPtr)
	{
		string message("CommandController가 초기화 되지 않아 심각한 오류를 처리할 수 없습니다.");
		errorMessageVec.push_back(message);
		cout << "CommandController가 초기화 되지 않아 심각한 오류 에러코드 = [ " << EnumErrorCode(code) << " ]를 처리할 수 없습니다." << endl;
	}
	else
	{
		if (false == mCommandPtr->DeleteClientSocket(*client))
		{
			cout << "에러가 발생한 클라이언트를 채널 혹은 room에서 제거하지 못했습니다." << endl;
			string message("에러가 발생한 클라이언트를 채널 혹은 room에서 제거하지 못했습니다.");
			errorMessageVec.push_back(message);
		}
		else
		{
			delete client;
			string message("스레드를 강제 종료 시킵니다.");
			errorMessageVec.push_back(message);
			cout << "스레드를 강제 종료 시킵니다." << endl;
		}
	}
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	_endthreadex(0);
	return code;
}

EnumErrorCode CErrorHandler::TakeNullLinkError(EnumErrorCode code)
{
	vector<string> errorMessageVec;
	GetErrorLevel(ErrorLevel::Normal, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	string message("client가 없는 에러코드");
	errorMessageVec.push_back(message);
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	cout << "client가 없는 에러코드 = " << EnumErrorCode(code) << endl;
	//_endthreadex(0);
	return code;
}

EnumErrorCode CErrorHandler::TakeSucces(EnumErrorCode code)
{
	vector<string> errorMessageVec;
	GetErrorLevel(ErrorLevel::Succes, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	cout << "성공 코드 = " << EnumErrorCode(code) << endl;
	return code;
}

EnumErrorCode CErrorHandler::TakeError(EnumErrorCode code, CLink * client)
{
	cout << "에러코드 = " << EnumErrorCode(code) << endl;
	//_endthreadex(0);
	if (nullptr == client)
	{
		return code;
	}
	vector<string> errorMessageVec;
	GetErrorLevel(ErrorLevel::Normal, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	GetErrorMemberInfo(client, errorMessageVec);
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	return code;
}

CErrorHandler::CErrorHandler():mCommandPtr(nullptr)
{
	cout << "ErrorHandler 생성자 호출" << endl;
}


CErrorHandler::~CErrorHandler()
{
}

bool CErrorHandler::GetErrorMemberInfo(CLink * client, vector<string>& memberInfoVec)
{
	if (nullptr == client)
	{
		string message("client가 초기화 되지 않아 심각한 오류를 처리할 수 없습니다.");
		memberInfoVec.push_back(message);
		return false;
	}
	char chPk[10];
	_itoa_s(client->GetMyPKNumber(), chPk, 10);
	const char* chName = client->GetMyName();
	memberInfoVec.push_back(string(chPk));
	memberInfoVec.push_back(string(chName));
	return true;
}

void CErrorHandler::GetErrorLevel(ErrorLevel level, vector<string>& memberInfoVec)
{
	string strLevel;
	switch (level)
	{
	case ErrorLevel::Serious:
		strLevel = "[높음]";
		break;
	case ErrorLevel::Normal:
		strLevel = "[보통]";
		break;
	case ErrorLevel::Low:
		strLevel = "[낮음]";
		break;
	case ErrorLevel::Succes:
		strLevel = "[성공]";
		break;
	default:
		strLevel = "[알수없음]";
		break;
	}
	memberInfoVec.push_back(strLevel);
}

void CErrorHandler::GetErrorCode(EnumErrorCode code, vector<string>& memberInfoVec)
{
	char chCode[10];
	_itoa_s(code, chCode, 10);
	memberInfoVec.push_back(string(chCode));
}

CErrorHandler * CErrorHandler::GetInstance()
{
	if (nullptr == ErrorHandStatic)
	{
		ErrorHandStatic = new CErrorHandler();
	}
	return ErrorHandStatic;
}

void CErrorHandler::setCommand(CCommandController * command)
{
	if (nullptr == mCommandPtr)
	{
		mCommandPtr = command;
	}
}

EnumErrorCode CErrorHandler::ErrorHandler(EnumErrorCode code, CLink * client)
{
	if (0 == code % 2)
	{
		return TakeSucces(code);
	}
	if (nullptr == client)
	{
		return TakeNullLinkError(code);
	}

	switch (code)
	{
	case ERROR_INIT_MONEY:
	case ERROR_RECV:
	case ERROR_SEND:
		return CriticalError(code, client);
	default:
		return TakeError(code, client);
	}
}
