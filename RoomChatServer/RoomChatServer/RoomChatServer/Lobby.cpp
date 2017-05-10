#define _CRT_SECURE_NO_WARNINGS
#include "Lobby.h"

bool CLobby::Login(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	g_MS.set_message("ID : ");
	int sendRecvSize = g_MS.ByteSize();
	actionNetWork.sendn(clientSocket, g_MS);
	actionNetWork.recvn(clientSocket, g_MS);
	id = g_MS.message();
	if (0 == id.compare("9"))
		return false;
	g_MS.set_message("PW : ");
	sendRecvSize = g_MS.ByteSize();
	actionNetWork.sendn(clientSocket, g_MS);
	actionNetWork.recvn(clientSocket, g_MS);
	pw = g_MS.message();
	if (0 == pw.compare("9"))
		return false;
	if (ReadHandler.CheckIDandPassWord(id, pw))
	{
		cout << "�α��� ����" << endl;
		g_MS.set_message("�α��� ���� �ϼ̽��ϴ�. ��ſ� ��ȭ �Ǽ���.");
		sendRecvSize = g_MS.ByteSize();
		actionNetWork.sendn(clientSocket, g_MS);
		g_MS.set_message(id.c_str());
		return true;
	}
	cout << "�α��� ����" << endl;
	g_MS.set_message("ID or PW ���� �Դϴ�.");
	sendRecvSize = g_MS.ByteSize();
	actionNetWork.sendn(clientSocket, g_MS);
	
	return false;
}

bool CLobby::JoinMember(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	g_MS.set_message("���ϴ� ID : ");
	actionNetWork.sendn(clientSocket, g_MS);
	actionNetWork.recvn(clientSocket, g_MS);
	id = g_MS.message();
	if (0 == id.compare("9"))
		return false;
	g_MS.set_message("PW �Է� : ");
	actionNetWork.sendn(clientSocket, g_MS);
	actionNetWork.recvn(clientSocket, g_MS);
	pw = g_MS.message();
	if (0 == pw.compare("9"))
		return false;
	if (!ReadHandler.CheckOverlapID(id))
	{
		cout << "id �ߺ� �Դϴ�." << endl;
		g_MS.set_message("id �ߺ� �Դϴ�.");
		actionNetWork.sendn(clientSocket, g_MS);
		return false;
	}
	if (WriteHandler.write("test.txt", 2, id, pw))
	{
		cout << "ȸ������ ����" << endl;
		g_MS.set_message("ȸ������ ���� �߽��ϴ�. �α��� ���ּ���.");
		actionNetWork.sendn(clientSocket, g_MS);
		return true;
	}
	else
	{
		cout << "ȸ������ ����" << endl;
		g_MS.set_message("ȸ������ ���� �Դϴ�.");
		actionNetWork.sendn(clientSocket, g_MS);
		return false;
	}
	return false;
}

int CLobby::ChooseMenu(string message, SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	switch (message[0])
	{
	case '1':
		cout << "�α���" << endl;
		g_MS.set_message("�α��� ȭ�� �Դϴ�. 9���Է� : ���");
		actionNetWork.sendn(clientSocket, g_MS);
		return 1;
	case '2':
		cout << "ȸ������" << endl;
		g_MS.set_message("ȸ������ ȭ�� �Դϴ�. 9���Է� : ���");
		actionNetWork.sendn(clientSocket, g_MS);
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
	g_MS.set_message("Welcome: Enter 1: Login / Enter 2: Join / Enter 9: Cancel");
	actionNetWork.sendn(clientSocket, g_MS);
}
