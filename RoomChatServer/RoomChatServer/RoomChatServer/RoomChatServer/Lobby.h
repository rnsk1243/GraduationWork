#pragma once
#include"ActionNetWork.h"
#include<vector>

class CLobby
{
	MessageStruct MS;
	int Login(SOCKET& clientSocket, CActionNetWork& actionNetWork, vector<string>& tempUserInfo);
	int JoinMember(SOCKET& clientSocket, CActionNetWork& actionNetWork, vector<string>& tempUserInfo);
	int ChooseMenu(char* message, SOCKET & clientSocket, CActionNetWork & actionNetWork);
	int SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork);
	int NextUserNum;
public:
	CLobby(int NextUserNum_):NextUserNum(NextUserNum_){}
	CLobby(const CLobby&) = delete;
	CLobby& operator=(const CLobby&) = delete;
	~CLobby(){}
	MessageStruct& getMessageStruct() { return MS; }	
	int ActionServiceLobby(SOCKET& clientSocket, CActionNetWork& actionNetWork, vector<string>& tempUserInfo);
};

