#pragma once
#include"ActionNetWork.h"
#include<vector>

class CLobby
{
	int Login(SOCKET& clientSocket, CActionNetWork& actionNetWork, vector<string>& tempUserInfo);
	int JoinMember(SOCKET& clientSocket, CActionNetWork& actionNetWork, vector<string>& tempUserInfo);
	int ChooseMenu(const char* message, SOCKET & clientSocket, CActionNetWork & actionNetWork);
	int SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork);
	int NextUserNum;
public:
	CLobby(int NextUserNum_):NextUserNum(NextUserNum_){}
	CLobby(const CLobby&) = delete;
	CLobby& operator=(const CLobby&) = delete;
	~CLobby(){}
	int ActionServiceLobby(SOCKET& clientSocket, CActionNetWork& actionNetWork, vector<string>& tempUserInfo);
};

