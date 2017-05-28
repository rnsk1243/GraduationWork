#define _CRT_SECURE_NO_WARNINGS
#include "Lobby.h"
#include"ErrorHandler.h"
#include"ReadHandler.h"
#include"WriteHandler.h"

int CLobby::Login(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	// ID ����
	actionNetWork.askClient(clientSocket, MS, "ID : ");
	id = MS.message;
	if (0 == id.compare("9"))
		return CErrorHandler::ErrorHandler(Cancel);

	// PW ����
	actionNetWork.askClient(clientSocket, MS, "PW : ");
	pw = MS.message;
	if (0 == pw.compare("9"))
		return CErrorHandler::ErrorHandler(Cancel);
	if (ReadHandlerStatic->Search("MemberInfo.txt", false, 2, id, pw))
	{
		cout << "�α��� ����" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "�α��� ���� �ϼ̽��ϴ�. ��ſ� ��ȭ �Ǽ���.");
		strcpy(MS.message, id.c_str());
		return CErrorHandler::ErrorHandler(SUCCES_LOGIN);
	}
	cout << "���̵� Ȥ�� ��й�ȣ�� Ʋ���ϴ�." << endl;
	actionNetWork.notificationClient(clientSocket, MS, "���̵� Ȥ�� ��й�ȣ�� Ʋ���ϴ�.");
	strcpy(MS.message, id.c_str());
	return CErrorHandler::ErrorHandler(ERROR_LOGIN);
}


int CLobby::JoinMember(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	// ID ����
	if (SUCCES_ASKCLIENT != actionNetWork.askClient(clientSocket, MS, "���ϴ� ID : "))
		return CErrorHandler::ErrorHandler(ERROR_RECV);
	id = MS.message;
	if (0 == id.compare("9"))
		return CErrorHandler::ErrorHandler(Cancel);

	if(SUCCES_ASKCLIENT != actionNetWork.askClient(clientSocket, MS, "PW �Է� : "))
		return CErrorHandler::ErrorHandler(ERROR_RECV);
	pw = MS.message;
	if (0 == pw.compare("9"))
		return CErrorHandler::ErrorHandler(Cancel);

	if (ReadHandlerStatic->Search("MemberInfo.txt", false, 1, id))
	{
		cout << "id �ߺ� �Դϴ�." << endl;
		actionNetWork.notificationClient(clientSocket, MS, "id �ߺ� �Դϴ�.");
		return CErrorHandler::ErrorHandler(OVERLAPID);
	}

	if (WriteHandlerStatic->write("MemberInfo.txt", 2, id, pw) && WriteHandlerStatic->write("MemberCardInfo.txt", 1, id))
	{
		cout << "ȸ������ ����" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "ȸ������ ���� �߽��ϴ�. �α��� ���ּ���.");
		return CErrorHandler::ErrorHandler(SUCCES_JOIN);
	}
	else
	{
		cout << "ȸ������ ����" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "ȸ������ ���� �Դϴ�.");
		return CErrorHandler::ErrorHandler(ERROR_JOIN);
	}
	return CErrorHandler::ErrorHandler(ERROR_EXCEPTION);
}

int CLobby::ChooseMenu(char * message, SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	switch (message[0])
	{
	case '1':
		cout << "�α���" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "�α��� ȭ�� �Դϴ�. 9���Է� : ���");
		return 1;
	case '2':
		cout << "ȸ������" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "ȸ������ ȭ�� �Դϴ�. 9���Է� : ���");
		return 2;
	case '9':
		cout << "���� �޴��� ���ư���" << endl;
		SendMenuInfo(clientSocket, actionNetWork);
		return 9;
	default:

		break;
	}
	return CErrorHandler::ErrorHandler(ERROR_MENUOUT);
}

int CLobby::SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	return actionNetWork.notificationClient(clientSocket, MS, "ȯ���մϴ�. 1���Է� : �α��� / 2���Է� : ȸ������ / 9���Է� : ���");;
}

int CLobby::ActionServiceLobby(SOCKET& clientSocket, CActionNetWork& actionNetWork)
{
	int resultLoginFunc = 0;
	SendMenuInfo(clientSocket, actionNetWork);
	actionNetWork.recvn(clientSocket, getMessageStruct());
	int choose = ChooseMenu(getMessageStruct().message, clientSocket, actionNetWork);
	switch (choose)
	{
	case 1:
		resultLoginFunc = Login(clientSocket, actionNetWork);
		if (SUCCES_LOGIN == resultLoginFunc)
		{
			return SUCCES_LOGIN;
		}
		else if (Cancel == resultLoginFunc)
		{
			return Cancel;
		}
		else if (ERROR_LOGIN == resultLoginFunc)
		{
			return ERROR_LOGIN;
		}
		return ERROR_RECV;
	case 2:
		return JoinMember(clientSocket, actionNetWork);
	case 9:
		return Cancel;
	default:
		return ERROR_WRONG_INPUT;
	}
	return ERROR_SEND;
}
