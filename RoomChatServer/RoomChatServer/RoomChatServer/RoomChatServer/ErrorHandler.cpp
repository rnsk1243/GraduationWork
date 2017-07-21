#include "ErrorHandler.h"
#include"Link.h"
#include"CommandController.h"



EnumErrorCode CErrorHandler::CriticalError(EnumErrorCode code, CLink * client)
{
	cout << "[�ɰ��� ���� �߻�] �����ڵ� = [ " << EnumErrorCode(code) << " ]" << endl;
	if (nullptr == client)
	{
		cout << "client�� �ʱ�ȭ ���� �ʾ� �ɰ��� ���� �����ڵ� = [ " << EnumErrorCode(code) << " ]�� ó���� �� �����ϴ�." << endl;
		return code;
	}
	if (nullptr == mCommandPtr)
	{
		cout << "CommandController�� �ʱ�ȭ ���� �ʾ� �ɰ��� ���� �����ڵ� = [ " << EnumErrorCode(code) << " ]�� ó���� �� �����ϴ�." << endl;
		return code;
	}
	else
	{
		if (mCommandPtr->DeleteClientSocket(*client))
		{
			cout << "�����带 ���� ���� ��ŵ�ϴ�." << endl;
			_endthreadex(0);
			return code;
		}
		else
		{
			cout << "������ �߻��� Ŭ���̾�Ʈ�� ä�� Ȥ�� room���� �������� ���߽��ϴ�." << endl;
			return code;
		}
	}
	return code;
}

EnumErrorCode CErrorHandler::TakeNullLinkError(EnumErrorCode code)
{
	cout << "client�� ���� �����ڵ� = " << EnumErrorCode(code) << endl;
	//_endthreadex(0);
	return code;
}

EnumErrorCode CErrorHandler::TakeSucces(EnumErrorCode code)
{
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

	return code;
}

CErrorHandler::CErrorHandler():mCommandPtr(nullptr)
{
	cout << "ErrorHandler ������ ȣ��" << endl;
}


CErrorHandler::~CErrorHandler()
{
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
