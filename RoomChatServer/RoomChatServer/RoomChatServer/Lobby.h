#pragma once
#include"ReadHandler.h"
#include"WriteHandler.h"
#include"ActionNetWork.h"

class CLobby
{
	CReadHandler ReadHandler;
	CWriteHandler WriteHandler;
	g_Message g_MS;
	CLobby(const CLobby&);
	CLobby& operator=(const CLobby&);
public:
	CLobby(){}
	~CLobby(){}
	bool Login(SOCKET& clientSocket, CActionNetWork& actionNetWork);
	bool JoinMember(SOCKET& clientSocket, CActionNetWork& actionNetWork);
	int ChooseMenu(string message, SOCKET & clientSocket, CActionNetWork & actionNetWork);
	void SendMenuInfo(SOCKET & clientSocket, CActionNetWork & actionNetWork);
	g_Message& getMessageStruct() { return g_MS; }
};

