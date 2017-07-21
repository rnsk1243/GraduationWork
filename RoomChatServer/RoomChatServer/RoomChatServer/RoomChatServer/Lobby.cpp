#define _CRT_SECURE_NO_WARNINGS
#include "Lobby.h"
#include"ErrorHandler.h"
#include"ReadHandler.h"
#include"WriteHandler.h"
#include"Util.h"

int CLobby::Login(SOCKET & clientSocket, CActionNetWork & actionNetWork, vector<string>& tempUserInfo)
{
	string id, pw;
	// ID 묻기
	actionNetWork.AskClient(clientSocket, MS, "ID : ");
	id = MS.message;
	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	// PW 묻기
	actionNetWork.AskClient(clientSocket, MS, "PW : ");
	pw = MS.message;
	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);
	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 2, id, pw))
	{
		cout << "로그인 성공" << endl;
		actionNetWork.NotificationClient(clientSocket, MS, "로그인 성공 하셨습니다. 즐거운 대화 되세요.");
		strcpy(MS.message, id.c_str());
		return ErrorHandStatic->ErrorHandler(SUCCES_LOGIN);
	}
	cout << "아이디 혹은 비밀번호가 틀립니다." << endl;
	actionNetWork.NotificationClient(clientSocket, MS, "아이디 혹은 비밀번호가 틀립니다.");
	strcpy(MS.message, id.c_str());
	return ErrorHandStatic->ErrorHandler(ERROR_LOGIN);
}


int CLobby::JoinMember(SOCKET & clientSocket, CActionNetWork & actionNetWork, vector<string>& tempUserInfo)
{
	string id, pw, nextUserNum;
	nextUserNum = IntToString(NextUserNum);
	// ID 묻기
	if (SUCCES_ASKCLIENT != actionNetWork.AskClient(clientSocket, MS, "원하는 ID : "))
		return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
	id = MS.message;
	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	if(SUCCES_ASKCLIENT != actionNetWork.AskClient(clientSocket, MS, "PW 입력 : "))
		return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
	pw = MS.message;
	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 1, id))
	{
		cout << "id 중복 입니다." << endl;
		actionNetWork.NotificationClient(clientSocket, MS, "id 중복 입니다.");
		return ErrorHandStatic->ErrorHandler(OVERLAPID);
	}
	
	if (WriteHandlerStatic->Write(NameMemberInfoTxt.c_str(), 3, nextUserNum, id, pw) /*&& WriteHandlerStatic->write(NameMemberCardInfoTxt.c_str(), 2, nextUserNum, StartCardInventory)*/)
	{
		if (NextUserNum > JoinMemberTxtCapacity)
		{
			WriteHandlerStatic->Write(NameMemberCardInfoTxt.c_str(), 2, nextUserNum, StartCardInventory);
			WriteHandlerStatic->Write(NameMemberGoodsTxt.c_str(), 2, nextUserNum, StartGoodsInventory);
		}
		cout << "회원가입 성공" << endl;
		WriteHandlerStatic->WriteNextJoinUserNum(MakeNextJoinNumberTxt, NextUserNum);
		++NextUserNum;
		actionNetWork.NotificationClient(clientSocket, MS, "회원가입 성공 했습니다. 로그인 해주세요.");
		return SUCCES_JOIN;
	}
	else
	{
		cout << "회원가입 실패" << endl;
		actionNetWork.NotificationClient(clientSocket, MS, "회원가입 실패 입니다.");
		return ErrorHandStatic->ErrorHandler(ERROR_JOIN);
	}
	return ErrorHandStatic->ErrorHandler(ERROR_EXCEPTION);
}

int CLobby::ChooseMenu(char * message, SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	switch (message[0])
	{
	case '1':
		cout << "로그인" << endl;
		if (ERROR_NULL_LINK_SEND == actionNetWork.NotificationClient(clientSocket, MS, "로그인 화면 입니다. 9번입력 : 취소"))
		{
			return ERROR_NULL_LINK_SEND;
		}
		return 1;
	case '2':
		cout << "회원가입" << endl;
		if (ERROR_NULL_LINK_SEND == actionNetWork.NotificationClient(clientSocket, MS, "회원가입 화면 입니다. 9번입력 : 취소"))
		{
			return ERROR_NULL_LINK_SEND;
		}
		return 2;
	case '9':
		cout << "이전 메뉴로 돌아가기" << endl;
		if (ERROR_NULL_LINK_SEND == SendMenuInfo(clientSocket, actionNetWork))
		{
			return ERROR_NULL_LINK_SEND;
		}
		return 9;
	default:
		cout << "잘 못 입력" << endl;
		break;
	}
	return ErrorHandStatic->ErrorHandler(ERROR_MENUOUT);
}

int CLobby::SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork)
{
	return actionNetWork.NotificationClient(clientSocket, MS, "환영합니다. 1번입력 : 로그인 / 2번입력 : 회원가입 / 9번입력 : 취소");;
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
