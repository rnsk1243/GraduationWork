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
	if (SUCCES_SEND == actionNetWork.Sendn(clientSocket, "ID : "))
	{
		actionNetWork.Recvn(clientSocket, id);
	}
	
	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	// PW ����
	if (SUCCES_SEND == actionNetWork.Sendn(clientSocket, "PW : "))
	{
		actionNetWork.Recvn(clientSocket, pw);
	}
	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);
	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 2, id, pw))
	{
		cout << "�α��� ����" << endl;
		actionNetWork.Sendn(clientSocket, "�α��� ���� �ϼ̽��ϴ�. ��ſ� ��ȭ �Ǽ���.");
		return ErrorHandStatic->ErrorHandler(SUCCES_LOGIN);
	}
	actionNetWork.Sendn(clientSocket, "���̵� Ȥ�� ��й�ȣ�� Ʋ���ϴ�.");
	return ErrorHandStatic->ErrorHandler(ERROR_LOGIN);
}


int CLobby::JoinMember(SOCKET & clientSocket, CActionNetWork & actionNetWork, vector<string>& tempUserInfo)
{
	string id, pw, nextUserNum;
	nextUserNum = IntToString(NextUserNum);
	// ID ����
	if (SUCCES_SEND == actionNetWork.Sendn(clientSocket, "���ϴ� ID : "))
	{
		actionNetWork.Recvn(clientSocket, id);
	}
	
	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	if (SUCCES_SEND == actionNetWork.Sendn(clientSocket, "PW �Է� : "))
	{
		actionNetWork.Recvn(clientSocket, pw);
	}
	
	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 1, id))
	{
		cout << "id �ߺ� �Դϴ�." << endl;
		actionNetWork.Sendn(clientSocket, "id �ߺ� �Դϴ�.");
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
		actionNetWork.Sendn(clientSocket, "ȸ������ ���� �߽��ϴ�. �α��� ���ּ���.");
		return SUCCES_JOIN;
	}
	else
	{
		cout << "ȸ������ ����" << endl;
		actionNetWork.Sendn(clientSocket, "ȸ������ ���� �Դϴ�.");
		return ErrorHandStatic->ErrorHandler(ERROR_JOIN);
	}
	return ErrorHandStatic->ErrorHandler(ERROR_EXCEPTION);
}

int CLobby::ChooseMenu(const char * message, SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	switch (message[0])
	{
	case '1':
		cout << "�α���" << endl;
		actionNetWork.Sendn(clientSocket, "�α��� ȭ�� �Դϴ�. 9���Է� : ���"); //AskClient(clientSocket, MS, "ID : ");
		return 1;
	case '2':
		cout << "ȸ������" << endl;
		actionNetWork.Sendn(clientSocket, "ȸ������ ȭ�� �Դϴ�. 9���Է� : ���");
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
	return actionNetWork.Sendn(clientSocket, "ȯ���մϴ�. 1���Է� : �α��� / 2���Է� : ȸ������ / 9���Է� : ���");;
}

int CLobby::ActionServiceLobby(SOCKET& clientSocket, CActionNetWork& actionNetWork, vector<string>& tempUserInfo)
{
	int resultLoginFunc = 0;
	SendMenuInfo(clientSocket, actionNetWork);
	string strMessage;
	if (ERROR_NULL_LINK_RECV == actionNetWork.Recvn(clientSocket, strMessage))
	{
		return ERROR_NULL_LINK_RECV;
	}
	int choose = ChooseMenu(strMessage.c_str(), clientSocket, actionNetWork);
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
