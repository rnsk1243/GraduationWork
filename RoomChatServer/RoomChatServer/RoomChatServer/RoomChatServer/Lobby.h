#pragma once
#include"ActionNetWork.h"

class CLobby
{
	MessageStruct MS;
	int Login(SOCKET& clientSocket, CActionNetWork& actionNetWork);
	int JoinMember(SOCKET& clientSocket, CActionNetWork& actionNetWork);
	int ChooseMenu(char* message, SOCKET & clientSocket, CActionNetWork & actionNetWork);
	int SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork);
public:
	CLobby(){}
	CLobby(const CLobby&) = delete;
	CLobby& operator=(const CLobby&) = delete;
	~CLobby(){}
	MessageStruct& getMessageStruct() { return MS; }	
	int ActionServiceLobby(SOCKET& clientSocket, CActionNetWork& actionNetWork);
};

