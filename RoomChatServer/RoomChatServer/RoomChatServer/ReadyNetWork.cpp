#include "ReadyNetWork.h"



CReadyNetWork::CReadyNetWork():
	hServSock(new SOCKET())
{
	WSADATA wsaData;
	SOCKADDR_IN servAddr;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		cout << "WSAStartup() error!" << endl;

	*hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (*hServSock == INVALID_SOCKET)
		cout << "socket() error" << endl;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(Port);

	if (bind(*hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		cout << "bind() error" << endl;
	if (listen(*hServSock, 5) == SOCKET_ERROR)
		cout << "listen() error" << endl;
	else
		cout << "listen..." << endl;
}


CReadyNetWork::~CReadyNetWork()
{
	closesocket(*hServSock);
}

void CReadyNetWork::Accept(SOCKET& hClientSock)
{
	if (hServSock == nullptr)
	{
		cout << "Accept Error" << endl;
		return;
	}
	// 소켓주소 메모리 할당
	SOCKADDR_IN* hClntAddr = new SOCKADDR_IN();
	int szClntAddr;
	// accept
	szClntAddr = sizeof(*hClntAddr);
	hClientSock = accept(*hServSock, (SOCKADDR*)hClntAddr, &szClntAddr);// list나 벡터에 저장
																		 //cout << "accept 에서의 주소 = " << hClntSock << endl;
	if (hClientSock == INVALID_SOCKET)
		cout << "accept() error" << endl;
	else
		cout << "Connected client" << endl;

}
