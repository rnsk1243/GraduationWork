#define _CRT_SECURE_NO_WARNINGS
#include "Lobby.h"

bool CLobby::Login(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	strcpy(MS.message, "ID : ");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
	actionNetWork.recvn(clientSocket, MS);
	id = MS.message;
	if (0 == id.compare("9"))
		return false;
	strcpy(MS.message, "PW : ");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
	actionNetWork.recvn(clientSocket, MS);
	pw = MS.message;
	if (0 == pw.compare("9"))
		return false;
	if (ReadHandler.CheckIDandPassWord(id, pw))
	{
		cout << "�α��� ����" << endl;
		strcpy(MS.message, "�α��� ���� �ϼ̽��ϴ�. ��ſ� ��ȭ �Ǽ���.");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		strcpy(MS.message, id.c_str());
		return true;
	}
	cout << "�α��� ����" << endl;
	strcpy(MS.message, "ID or PW ���� �Դϴ�.");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
	
	return false;
}

bool CLobby::JoinMember(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	strcpy(MS.message, "���ϴ� ID : ");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
	actionNetWork.recvn(clientSocket, MS);
	id = MS.message;
	if (0 == id.compare("9"))
		return false;
	strcpy(MS.message, "PW �Է� : ");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
	actionNetWork.recvn(clientSocket, MS);
	pw = MS.message;
	if (0 == pw.compare("9"))
		return false;
	if (!ReadHandler.CheckOverlapID(id))
	{
		cout << "id �ߺ� �Դϴ�." << endl;
		strcpy(MS.message, "id �ߺ� �Դϴ�.");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		return false;
	}
	if (WriteHandler.write("test.txt", 2, id, pw))
	{
		cout << "ȸ������ ����" << endl;
		strcpy(MS.message, "ȸ������ ���� �߽��ϴ�. �α��� ���ּ���.");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		return true;
	}
	else
	{
		cout << "ȸ������ ����" << endl;
		strcpy(MS.message, "ȸ������ ���� �Դϴ�.");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		return false;
	}
	return false;
}

int CLobby::ChooseMenu(char * message, SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	switch (message[0])
	{
	case '1':
		cout << "�α���" << endl;
		strcpy(MS.message, "�α��� ȭ�� �Դϴ�. 9���Է� : ���");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		return 1;
	case '2':
		cout << "ȸ������" << endl;
		strcpy(MS.message, "ȸ������ ȭ�� �Դϴ�. 9���Է� : ���");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		return 2;
	case '9':
		cout << "���� �޴��� ���ư���" << endl;
		SendMenuInfo(clientSocket, actionNetWork);
		return -1;
	default:
		return -1;
	}
	return -1;
}

void CLobby::SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	strcpy(MS.message, "ȯ���մϴ�. 1���Է� : �α��� / 2���Է� : ȸ������ / 9���Է� : ���");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
}
