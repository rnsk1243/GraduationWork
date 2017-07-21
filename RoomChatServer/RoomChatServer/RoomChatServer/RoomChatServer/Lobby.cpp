#define _CRT_SECURE_NO_WARNINGS
#include "Lobby.h"
#include"ErrorHandler.h"
#include"ReadHandler.h"
#include"WriteHandler.h"
#include"Util.h"

int CLobby::Login(SOCKET & clientSocket, CActionNetWork & actionNetWork, vector<string>& tempUserInfo)
{
	string id, pw;
	// ID ����
	actionNetWork.AskClient(clientSocket, MS, "ID : ");
	id = MS.message;
	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	// PW ����
	actionNetWork.AskClient(clientSocket, MS, "PW : ");
	pw = MS.message;
	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);
	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 2, id, pw))
	{
		cout << "�α��� ����" << endl;
		actionNetWork.NotificationClient(clientSocket, MS, "�α��� ���� �ϼ̽��ϴ�. ��ſ� ��ȭ �Ǽ���.");
		strcpy(MS.message, id.c_str());
		return ErrorHandStatic->ErrorHandler(SUCCES_LOGIN);
	}
	cout << "���̵� Ȥ�� ��й�ȣ�� Ʋ���ϴ�." << endl;
	actionNetWork.NotificationClient(clientSocket, MS, "���̵� Ȥ�� ��й�ȣ�� Ʋ���ϴ�.");
	strcpy(MS.message, id.c_str());
	return ErrorHandStatic->ErrorHandler(ERROR_LOGIN);
}


int CLobby::JoinMember(SOCKET & clientSocket, CActionNetWork & actionNetWork, vector<string>& tempUserInfo)
{
	string id, pw, nextUserNum;
	nextUserNum = IntToString(NextUserNum);
	// ID ����
	if (SUCCES_ASKCLIENT != actionNetWork.AskClient(clientSocket, MS, "���ϴ� ID : "))
		return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
	id = MS.message;
	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	if(SUCCES_ASKCLIENT != actionNetWork.AskClient(clientSocket, MS, "PW �Է� : "))
		return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
	pw = MS.message;
	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 1, id))
	{
		cout << "id �ߺ� �Դϴ�." << endl;
		actionNetWork.NotificationClient(clientSocket, MS, "id �ߺ� �Դϴ�.");
		return ErrorHandStatic->ErrorHandler(OVERLAPID);
	}
	
	if (WriteHandlerStatic->Write(NameMemberInfoTxt.c_str(), 3, nextUserNum, id, pw) /*&& WriteHandlerStatic->write(NameMemberCardInfoTxt.c_str(), 2, nextUserNum, StartCardInventory)*/)
	{
		if (NextUserNum > JoinMemberTxtCapacity)
		{
			WriteHandlerStatic->Write(NameMemberCardInfoTxt.c_str(), 2, nextUserNum, StartCardInventory);
			WriteHandlerStatic->Write(NameMemberGoodsTxt.c_str(), 2, nextUserNum, StartGoodsInventory);
		}
		cout << "ȸ������ ����" << endl;
		WriteHandlerStatic->WriteNextJoinUserNum(MakeNextJoinNumberTxt, NextUserNum);
		++NextUserNum;
		actionNetWork.NotificationClient(clientSocket, MS, "ȸ������ ���� �߽��ϴ�. �α��� ���ּ���.");
		return SUCCES_JOIN;
	}
	else
	{
		cout << "ȸ������ ����" << endl;
		actionNetWork.NotificationClient(clientSocket, MS, "ȸ������ ���� �Դϴ�.");
		return ErrorHandStatic->ErrorHandler(ERROR_JOIN);
	}
	return ErrorHandStatic->ErrorHandler(ERROR_EXCEPTION);
}

int CLobby::ChooseMenu(char * message, SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	switch (message[0])
	{
	case '1':
		cout << "�α���" << endl;
		if (ERROR_NULL_LINK_SEND == actionNetWork.NotificationClient(clientSocket, MS, "�α��� ȭ�� �Դϴ�. 9���Է� : ���"))
		{
			return ERROR_NULL_LINK_SEND;
		}
		return 1;
	case '2':
		cout << "ȸ������" << endl;
		if (ERROR_NULL_LINK_SEND == actionNetWork.NotificationClient(clientSocket, MS, "ȸ������ ȭ�� �Դϴ�. 9���Է� : ���"))
		{
			return ERROR_NULL_LINK_SEND;
		}
		return 2;
	case '9':
		cout << "���� �޴��� ���ư���" << endl;
		if (ERROR_NULL_LINK_SEND == SendMenuInfo(clientSocket, actionNetWork))
		{
			return ERROR_NULL_LINK_SEND;
		}
		return 9;
	default:
		cout << "�� �� �Է�" << endl;
		break;
	}
	return ErrorHandStatic->ErrorHandler(ERROR_MENUOUT);
}

int CLobby::SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	return actionNetWork.NotificationClient(clientSocket, MS, "ȯ���մϴ�. 1���Է� : �α��� / 2���Է� : ȸ������ / 9���Է� : ���");;
}

int CLobby::ActionServiceLobby(SOCKET& clientSocket, CActionNetWork& actionNetWork, vector<string>& tempUserInfo)
{
	int resultLoginFunc = 0;
	SendMenuInfo(clientSocket, actionNetWork);
	if (ERROR_NULL_LINK_RECV == actionNetWork.Recvn(clientSocket, getMessageStruct()))
	{
		return ERROR_NULL_LINK_RECV;
	}
	int choose = ChooseMenu(getMessageStruct().message, clientSocket, actionNetWork);
	switch (choose)
	{
	case 1:
		resultLoginFunc = Login(clientSocket, actionNetWork, tempUserInfo);
		if (SUCCES_LOGIN == resultLoginFunc)
		{
			return SUCCES_LOGIN;
		}
		else if (CANCLE == resultLoginFunc)
		{
			return CANCLE;
		}
		else if (ERROR_LOGIN == resultLoginFunc)
		{
			return ERROR_LOGIN;
		}
		return ERROR_NULL_LINK_RECV;
	case 2:
		return JoinMember(clientSocket, actionNetWork, tempUserInfo);
	case 9:
		return CANCLE;
	default:
		return ERROR_WRONG_INPUT;
	}
	return ERROR_NULL_LINK_SEND;
}
