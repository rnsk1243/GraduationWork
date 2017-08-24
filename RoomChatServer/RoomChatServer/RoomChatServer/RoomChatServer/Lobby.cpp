#define _CRT_SECURE_NO_WARNINGS
#include "Lobby.h"
#include"ErrorHandler.h"
#include"ReadHandler.h"
#include"WriteHandler.h"
#include"GuestLink.h"
#include"Util.h"

CLobby::CLobby(int NextUserNum_) :mNextUserNum(NextUserNum_) {}

void CLobby::AskedID(const CGuestLink * guest, string & id)
{
	guest->Sendn("ID : ");
	ListenerStatic->Recvn((guest->GetClientSocket()), id);
}

void CLobby::AskedPW(const CGuestLink * guest, string & pw)
{
	guest->Sendn("PW : ");
	ListenerStatic->Recvn(guest->GetClientSocket(), pw);
}

int CLobby::Login(const CGuestLink * guest , vector<string>& tempUserInfo)
{
	string id, pw;
	// ID 묻기
	AskedPW(guest, id);
	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	// PW 묻기
	AskedPW(guest, pw);
	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);
	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 2, id, pw))
	{
		cout << "로그인 성공" << endl;
		guest->Sendn("로그인 성공 하셨습니다. 즐거운 대화 되세요.");
		return ErrorHandStatic->ErrorHandler(SUCCES_LOGIN);
	}
	guest->Sendn("아이디 혹은 비밀번호가 틀립니다.");
	return ErrorHandStatic->ErrorHandler(ERROR_LOGIN);
}

int CLobby::JoinMember(const CGuestLink * guest, vector<string>& tempUserInfo)
{
	SetNextUserPKNumber();
	string id, pw, nextUserNum;
	nextUserNum = IntToString(mNextUserNum);
	// ID 묻기
	AskedID(guest, id);

	if (0 == id.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	AskedPW(guest, pw);

	if (0 == pw.compare("9"))
		return ErrorHandStatic->ErrorHandler(CANCLE);

	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 1, id))
	{
		cout << "id 중복 입니다." << endl;
		guest->Sendn("id 중복 입니다.");
		return ErrorHandStatic->ErrorHandler(OVERLAPID);
	}

	if (WriteHandlerStatic->Write(NameMemberInfoTxt.c_str(), 3, nextUserNum, id, pw) /*&& WriteHandlerStatic->write(NameMemberCardInfoTxt.c_str(), 2, nextUserNum, StartCardInventory)*/)
	{
		if (mNextUserNum > JoinMemberTxtCapacity)
		{
			WriteHandlerStatic->Write(NameMemberCardInfoTxt.c_str(), 2, nextUserNum, StartCardInventory);
			WriteHandlerStatic->Write(NameMemberGoodsTxt.c_str(), 2, nextUserNum, StartGoodsInventory);
		}
		cout << "회원가입 성공" << endl;
		WriteHandlerStatic->WriteNextJoinUserNum(MakeNextJoinNumberTxt, mNextUserNum);
		++mNextUserNum;
		guest->Sendn("회원가입 성공 했습니다. 로그인 해주세요.");
		return SUCCES_JOIN;
	}
	else
	{
		cout << "회원가입 실패" << endl;
		guest->Sendn("회원가입 실패 입니다.");
		return ErrorHandStatic->ErrorHandler(ERROR_JOIN);
	}
	return ErrorHandStatic->ErrorHandler(ERROR_EXCEPTION);
}


int CLobby::ChooseMenu(const char * message, const CGuestLink* guest)
{
	switch (message[0])
	{
	case '1':
		cout << "로그인" << endl;
		guest->Sendn("로그인 화면 입니다. 9번입력 : 취소"); //AskClient(clientSocket, MS, "ID : ");
		return 1;
	case '2':
		cout << "회원가입" << endl;
		guest->Sendn("회원가입 화면 입니다. 9번입력 : 취소");
		return 2;
	case '9':
		cout << "이전 메뉴로 돌아가기" << endl;
		guest->Sendn("환영합니다. 1번입력 : 로그인 / 2번입력 : 회원가입 / 9번입력 : 취소");
		return 9;
	default:
		cout << "잘 못 입력" << endl;
		break;
	}
	return ErrorHandStatic->ErrorHandler(ERROR_MENUOUT);
}

void CLobby::SetNextUserPKNumber()
{
	if (0 == mNextUserNum)
	{
		string strNextUserPKNumber = ReadHandlerStatic->GetNextUserNum(MakeNextJoinNumberTxt);
		mNextUserNum = stoi(strNextUserPKNumber);
	}
}

CLobby * CLobby::GetInstance(const int& NextUserNum)
{
	if (nullptr == LobbyStatic)
	{
		return new CLobby(NextUserNum);
	}
	return LobbyStatic;
}


int CLobby::ActionServiceLobby(const CGuestLink* guest, vector<string>& tempUserInfo)
{
	int resultLoginFunc = 0;
	guest->Sendn("환영합니다. 1번입력 : 로그인 / 2번입력 : 회원가입 / 9번입력 : 취소");

	string strMessage;
	ListenerStatic->Recvn(guest->GetClientSocket(), strMessage);

	int choose = ChooseMenu(strMessage.c_str(), guest);
	switch (choose)
	{
	case 1:
		resultLoginFunc = Login(guest, tempUserInfo);
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
		return JoinMember(guest, tempUserInfo);
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
