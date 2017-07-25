#include "ErrorHandler.h"
#include"Link.h"
#include"CommandController.h"
#include"WriteHandler.h"


EnumErrorCode CErrorHandler::CriticalError(EnumErrorCode code, CLink * client)
{
	//cout << "[�ɰ��� ���� �߻�] �����ڵ� = [ " << EnumErrorCode(code) << " ]" << endl;
	vector<string> errorMessageVec;
	GetErrorLevel(ErrorLevel::Serious, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	GetErrorMemberInfo(client, errorMessageVec);
	if (nullptr == client)
	{
		cout << "client�� �ʱ�ȭ ���� �ʾ� �ɰ��� ���� �����ڵ� = [ " << EnumErrorCode(code) << " ]�� ó���� �� �����ϴ�." << endl;
	}else if (nullptr == mCommandPtr)
	{
		string message("CommandController�� �ʱ�ȭ ���� �ʾ� �ɰ��� ������ ó���� �� �����ϴ�.");
		errorMessageVec.push_back(message);
		cout << "CommandController�� �ʱ�ȭ ���� �ʾ� �ɰ��� ���� �����ڵ� = [ " << EnumErrorCode(code) << " ]�� ó���� �� �����ϴ�." << endl;
	}
	else
	{
		if (false == mCommandPtr->DeleteClientSocket(*client))
		{
			cout << "������ �߻��� Ŭ���̾�Ʈ�� ä�� Ȥ�� room���� �������� ���߽��ϴ�." << endl;
			string message("������ �߻��� Ŭ���̾�Ʈ�� ä�� Ȥ�� room���� �������� ���߽��ϴ�.");
			errorMessageVec.push_back(message);
		}
		else
		{
			delete client;
			string message("�����带 ���� ���� ��ŵ�ϴ�.");
			errorMessageVec.push_back(message);
			cout << "�����带 ���� ���� ��ŵ�ϴ�." << endl;
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
	string message("client�� ���� �����ڵ�");
	errorMessageVec.push_back(message);
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	cout << "client�� ���� �����ڵ� = " << EnumErrorCode(code) << endl;
	//_endthreadex(0);
	return code;
}

EnumErrorCode CErrorHandler::TakeSucces(EnumErrorCode code)
{
	vector<string> errorMessageVec;
	GetErrorLevel(ErrorLevel::Succes, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	cout << "���� �ڵ� = " << EnumErrorCode(code) << endl;
	return code;
}

EnumErrorCode CErrorHandler::TakeError(EnumErrorCode code, CLink * client)
{
	cout << "�����ڵ� = " << EnumErrorCode(code) << endl;
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
	cout << "ErrorHandler ������ ȣ��" << endl;
}


CErrorHandler::~CErrorHandler()
{
}

bool CErrorHandler::GetErrorMemberInfo(CLink * client, vector<string>& memberInfoVec)
{
	if (nullptr == client)
	{
		string message("client�� �ʱ�ȭ ���� �ʾ� �ɰ��� ������ ó���� �� �����ϴ�.");
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
		strLevel = "[����]";
		break;
	case ErrorLevel::Normal:
		strLevel = "[����]";
		break;
	case ErrorLevel::Low:
		strLevel = "[����]";
		break;
	case ErrorLevel::Succes:
		strLevel = "[����]";
		break;
	default:
		strLevel = "[�˼�����]";
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
