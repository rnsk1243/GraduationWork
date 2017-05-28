#define _CRT_SECURE_NO_WARNINGS
#include "Lobby.h"
#include"ErrorHandler.h"
#include"ReadHandler.h"
#include"WriteHandler.h"

int CLobby::Login(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	// ID 묻기
	actionNetWork.askClient(clientSocket, MS, "ID : ");
	id = MS.message;
	if (0 == id.compare("9"))
		return CErrorHandler::ErrorHandler(Cancel);

	// PW 묻기
	actionNetWork.askClient(clientSocket, MS, "PW : ");
	pw = MS.message;
	if (0 == pw.compare("9"))
		return CErrorHandler::ErrorHandler(Cancel);
	if (ReadHandlerStatic->Search("MemberInfo.txt", false, 2, id, pw))
	{
		cout << "로그인 성공" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "로그인 성공 하셨습니다. 즐거운 대화 되세요.");
		strcpy(MS.message, id.c_str());
		return CErrorHandler::ErrorHandler(SUCCES_LOGIN);
	}
	cout << "아이디 혹은 비밀번호가 틀립니다." << endl;
	actionNetWork.notificationClient(clientSocket, MS, "아이디 혹은 비밀번호가 틀립니다.");
	strcpy(MS.message, id.c_str());
	return CErrorHandler::ErrorHandler(ERROR_LOGIN);
}


int CLobby::JoinMember(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	string id, pw;
	// ID 묻기
	if (SUCCES_ASKCLIENT != actionNetWork.askClient(clientSocket, MS, "원하는 ID : "))
		return CErrorHandler::ErrorHandler(ERROR_RECV);
	id = MS.message;
	if (0 == id.compare("9"))
		return CErrorHandler::ErrorHandler(Cancel);

	if(SUCCES_ASKCLIENT != actionNetWork.askClient(clientSocket, MS, "PW 입력 : "))
		return CErrorHandler::ErrorHandler(ERROR_RECV);
	pw = MS.message;
	if (0 == pw.compare("9"))
		return CErrorHandler::ErrorHandler(Cancel);

	if (ReadHandlerStatic->Search("MemberInfo.txt", false, 1, id))
	{
		cout << "id 중복 입니다." << endl;
		actionNetWork.notificationClient(clientSocket, MS, "id 중복 입니다.");
		return CErrorHandler::ErrorHandler(OVERLAPID);
	}

	if (WriteHandlerStatic->write("MemberInfo.txt", 2, id, pw) && WriteHandlerStatic->write("MemberCardInfo.txt", 1, id))
	{
		cout << "회원가입 성공" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "회원가입 성공 했습니다. 로그인 해주세요.");
		return CErrorHandler::ErrorHandler(SUCCES_JOIN);
	}
	else
	{
		cout << "회원가입 실패" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "회원가입 실패 입니다.");
		return CErrorHandler::ErrorHandler(ERROR_JOIN);
	}
	return CErrorHandler::ErrorHandler(ERROR_EXCEPTION);
}

int CLobby::ChooseMenu(char * message, SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	switch (message[0])
	{
	case '1':
		cout << "로그인" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "로그인 화면 입니다. 9번입력 : 취소");
		return 1;
	case '2':
		cout << "회원가입" << endl;
		actionNetWork.notificationClient(clientSocket, MS, "회원가입 화면 입니다. 9번입력 : 취소");
		return 2;
	case '9':
		cout << "이전 메뉴로 돌아가기" << endl;
		SendMenuInfo(clientSocket, actionNetWork);
		return 9;
	default:

		break;
	}
	return CErrorHandler::ErrorHandler(ERROR_MENUOUT);
}

int CLobby::SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	return actionNetWork.notificationClient(clientSocket, MS, "환영합니다. 1번입력 : 로그인 / 2번입력 : 회원가입 / 9번입력 : 취소");;
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
