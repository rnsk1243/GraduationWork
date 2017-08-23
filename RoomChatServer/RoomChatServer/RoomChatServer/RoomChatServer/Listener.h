#pragma once
#include"Link.h"
#include"CommandController.h"

//typedef vector<SOCKET> SocketVec;
//typedef SocketVec::iterator SocketVecIt;

class CListener
{
public:
	CListener();
	CListener(const CListener&) = delete;
	CListener& operator=(const CListener&) = delete;
	~CListener();
	// 클라이언트에게 답장 받기
	int Recvn(const SOCKET& socket, string& strMessage, int flags = 0);
};

