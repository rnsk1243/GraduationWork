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
		cout << "로그인 성공" << endl;
		strcpy(MS.message, "로그인 성공 하셨습니다. 즐거운 대화 되세요.");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		strcpy(MS.message, id.c_str());
		return true;
	}
	cout << "로그인 실패" << endl;
	strcpy(MS.message, "ID or PW 오류 입니다.");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
	
	return false;
}

bool CLobby::JoinMember(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	strcpy(MS.message, "원하는 ID : ");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
	actionNetWork.recvn(clientSocket, MS);
	id = MS.message;
	if (0 == id.compare("9"))
		return false;
	strcpy(MS.message, "PW 입력 : ");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
	actionNetWork.recvn(clientSocket, MS);
	pw = MS.message;
	if (0 == pw.compare("9"))
		return false;
	if (!ReadHandler.CheckOverlapID(id))
	{
		cout << "id 중복 입니다." << endl;
		strcpy(MS.message, "id 중복 입니다.");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		return false;
	}
	if (WriteHandler.write("test.txt", 2, id, pw))
	{
		cout << "회원가입 성공" << endl;
		strcpy(MS.message, "회원가입 성공 했습니다. 로그인 해주세요.");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		return true;
	}
	else
	{
		cout << "회원가입 실패" << endl;
		strcpy(MS.message, "회원가입 실패 입니다.");
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
		cout << "로그인" << endl;
		strcpy(MS.message, "로그인 화면 입니다. 9번입력 : 취소");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
		return 1;
	case '2':
		cout << "회원가입" << endl;
		strcpy(MS.message, "회원가입 화면 입니다. 9번입력 : 취소");
		MS.sendRecvSize = strlen(MS.message);
		actionNetWork.sendn(clientSocket, MS);
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
	strcpy(MS.message, "환영합니다. 1번입력 : 로그인 / 2번입력 : 회원가입 / 9번입력 : 취소");
	MS.sendRecvSize = strlen(MS.message);
	actionNetWork.sendn(clientSocket, MS);
}
