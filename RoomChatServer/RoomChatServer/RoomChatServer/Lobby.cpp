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
		cout << "로그인 성공" << endl;
		g_MS.set_message("로그인 성공 하셨습니다. 즐거운 대화 되세요.");
		sendRecvSize = g_MS.ByteSize();
		actionNetWork.sendn(clientSocket, g_MS);
		g_MS.set_message(id.c_str());
		return true;
	}
	cout << "로그인 실패" << endl;
	g_MS.set_message("ID or PW 오류 입니다.");
	sendRecvSize = g_MS.ByteSize();
	actionNetWork.sendn(clientSocket, g_MS);
	
	return false;
}

bool CLobby::JoinMember(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	g_MS.set_message("원하는 ID : ");
	actionNetWork.sendn(clientSocket, g_MS);
	actionNetWork.recvn(clientSocket, g_MS);
	id = g_MS.message();
	if (0 == id.compare("9"))
		return false;
	g_MS.set_message("PW 입력 : ");
	actionNetWork.sendn(clientSocket, g_MS);
	actionNetWork.recvn(clientSocket, g_MS);
	pw = g_MS.message();
	if (0 == pw.compare("9"))
		return false;
	if (!ReadHandler.CheckOverlapID(id))
	{
		cout << "id 중복 입니다." << endl;
		g_MS.set_message("id 중복 입니다.");
		actionNetWork.sendn(clientSocket, g_MS);
		return false;
	}
	if (WriteHandler.write("test.txt", 2, id, pw))
	{
		cout << "회원가입 성공" << endl;
		g_MS.set_message("회원가입 성공 했습니다. 로그인 해주세요.");
		actionNetWork.sendn(clientSocket, g_MS);
		return true;
	}
	else
	{
		cout << "회원가입 실패" << endl;
		g_MS.set_message("회원가입 실패 입니다.");
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
		cout << "로그인" << endl;
		g_MS.set_message("로그인 화면 입니다. 9번입력 : 취소");
		actionNetWork.sendn(clientSocket, g_MS);
		return 1;
	case '2':
		cout << "회원가입" << endl;
		g_MS.set_message("회원가입 화면 입니다. 9번입력 : 취소");
		actionNetWork.sendn(clientSocket, g_MS);
		return 2;
	case '9':
		cout << "이전 메뉴로 돌아가기" << endl;
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
