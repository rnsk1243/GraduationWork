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

	switch (code)
	{
	case ERROR_VALID_NUMBER_OUT_OF_RANGE:
	case ERROR_MAXCIPHER_OVER:
	case ERROR_CALC_CIPHER_MINUS_NUMBER:
	case ERROR_SAVE_TXT_CAPACITY_CIPHER:
	case ERROR_CIPHER_OUT_RANGE:
	case ERROR_INT_TO_ALPHABET_OUT_RANGE:
	case ERROR_SHARED_CHANNEL_COUNT_ZORO:
	case ERROR_CURSER_SIZE:
	case OVERLAPID:
	case ERROR_EXCEPTION:
	case ERROR_JOIN:
	case ERROR_MENUOUT:
	case ERROR_WRONG_INPUT:
	case ERROR_LOGIN:
	case CANCLE:
	case ERROR_COMPOSE_EVOUTION_CARD:
	case ERROR_SHARED_LINK_COUNT_ZORO:
	case ERROR_NULL_LINK_RECV:
	case ERROR_NULL_LINK_SEND:
		return TakeNullLinkError(code);
	case ERROR_RECV:
	case ERROR_SEND:
		return CriticalError(code, client);
	default:
		return TakeError(code, client);
	}
}
