#define _CRT_SECURE_NO_WARNINGS
#include "Lobby.h"
#include"ErrorHandler.h"
#include"ReadHandler.h"
#include"WriteHandler.h"
#include"GuestLink.h"
#include"Util.h"

int CLobby::Login(CGuestLink& guest , CListener& listener, vector<string>& tempUserInfo)
{
	string id, pw;
	// ID ����
	guest.Sendn("ID : ");
	listener.Recvn(guest.GetClientSocket(), id);
	
	
	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	// PW ����
	guest.Sendn("PW : ");
	listener.Recvn(guest.GetClientSocket(), pw);
	
	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);
	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 2, id, pw))
	{
		cout << "�α��� ����" << endl;
		guest.Sendn("�α��� ���� �ϼ̽��ϴ�. ��ſ� ��ȭ �Ǽ���.");
		return ErrorHandStatic->ErrorHandler(SUCCES_LOGIN);
	}
	guest.Sendn("���̵� Ȥ�� ��й�ȣ�� Ʋ���ϴ�.");
	return ErrorHandStatic->ErrorHandler(ERROR_LOGIN);
}


int CLobby::JoinMember(CGuestLink& guest, CListener& listener, vector<string>& tempUserInfo)
{
	string id, pw, nextUserNum;
	nextUserNum = IntToString(NextUserNum);
	// ID ����
	guest.Sendn("���ϴ� ID : ");
	listener.Recvn(guest.GetClientSocket(), id);
	
	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	guest.Sendn("PW �Է� : ");
	listener.Recvn(guest.GetClientSocket(), pw);

	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 1, id))
	{
		cout << "id �ߺ� �Դϴ�." << endl;
		guest.Sendn("id �ߺ� �Դϴ�.");
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
		guest.Sendn("ȸ������ ���� �߽��ϴ�. �α��� ���ּ���.");
		return SUCCES_JOIN;
	}
	else
	{
		cout << "ȸ������ ����" << endl;
		guest.Sendn("ȸ������ ���� �Դϴ�.");
		return ErrorHandStatic->ErrorHandler(ERROR_JOIN);
	}
	return ErrorHandStatic->ErrorHandler(ERROR_EXCEPTION);
}

int CLobby::ChooseMenu(const char * message, CGuestLink& guest)
{
	switch (message[0])
	{
	case '1':
		cout << "�α���" << endl;
		guest.Sendn("�α��� ȭ�� �Դϴ�. 9���Է� : ���"); //AskClient(clientSocket, MS, "ID : ");
		return 1;
	case '2':
		cout << "ȸ������" << endl;
		guest.Sendn("ȸ������ ȭ�� �Դϴ�. 9���Է� : ���");
		return 2;
	case '9':
		cout << "���� �޴��� ���ư���" << endl;
		guest.Sendn("ȯ���մϴ�. 1���Է� : �α��� / 2���Է� : ȸ������ / 9���Է� : ���");
		return 9;
	default:
		cout << "�� �� �Է�" << endl;
		break;
	}
	return ErrorHandStatic->ErrorHandler(ERROR_MENUOUT);
}


int CLobby::ActionServiceLobby(CGuestLink& guest, CListener& listener, vector<string>& tempUserInfo)
{
	int resultLoginFunc = 0;
	guest.Sendn("ȯ���մϴ�. 1���Է� : �α��� / 2���Է� : ȸ������ / 9���Է� : ���");

	string strMessage;
	listener.Recvn(guest.GetClientSocket(), strMessage);

	int choose = ChooseMenu(strMessage.c_str(), guest);
	switch (choose)
	{
	case 1:
		resultLoginFunc = Login(guest, listener, tempUserInfo);
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
		return JoinMember(guest, listener, tempUserInfo);
	case 9:
		return CANCLE;
	default:
		return ERROR_WRONG_INPUT;
	}
	return ERROR_NULL_LINK_SEND;
}

//void CLobby::PushGuest(const GuestLinkPtr & newGuest)
//{
//	ScopeLock<MUTEX> lock(mRAII_GuestMUTEX);
//	mGuestList.push_back(newGuest);
//}
//
//void CLobby::EraseGuest(const GuestLinkPtr & targetGuest)
//{
//	ScopeLock<MUTEX> lock(mRAII_GuestMUTEX);
//	GuestListIt targetIter = find(mGuestList.begin(), mGuestList.end(), targetGuest);
//	if (mGuestList.end() != targetIter)
//	{
//		mGuestList.erase(targetIter);
//	}
//}
