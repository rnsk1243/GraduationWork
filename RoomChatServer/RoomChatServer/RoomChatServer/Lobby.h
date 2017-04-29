#pragma once
#include"ReadHandler.h"
#include"WriteHandler.h"
#include"ActionNetWork.h"

class CLobby
{
	CReadHandler ReadHandler;
	CWriteHandler WriteHandler;
	MessageStruct MS;
	CLobby(const CLobby&);
	CLobby& operator=(const CLobby&);
	int Login(SOCKET& clientSocket, CActionNetWork& actionNetWork);
	int JoinMember(SOCKET& clientSocket, CActionNetWork& actionNetWork);
	int ChooseMenu(char* message, SOCKET & clientSocket, CActionNetWork & actionNetWork);
	int SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork);
public:
	CLobby(){}
	~CLobby(){}
	MessageStruct& getMessageStruct() { return MS; }	
	int ActionServiceLobby(SOCKET& clientSocket, CActionNetWork& actionNetWork);
};

