#include "MakeSocket.h"



CMakeSocket::CMakeSocket()
{
	WSADATA wsaData;
	SOCKADDR_IN servAddr;
	hServSock = new SOCKET();
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


CMakeSocket::~CMakeSocket()
{
	closesocket(*hServSock);
}

SOCKET * CMakeSocket::Accept()
{
	if (hServSock == nullptr)
	{
		cout << "Accept Error" << endl;
		return nullptr;
	}
	// 소켓&주소 메모리 할당
	SOCKET* hClientSock = new SOCKET();
	SOCKADDR_IN* hClntAddr = new SOCKADDR_IN();
	int szClntAddr;
	// accept
	szClntAddr = sizeof(*hClntAddr);
	*hClientSock = accept(*hServSock, (SOCKADDR*)hClntAddr, &szClntAddr);// list나 벡터에 저장
																		 //cout << "accept 에서의 주소 = " << hClntSock << endl;
	if (*hClientSock == INVALID_SOCKET)
		cout << "accept() error" << endl;
	else
		cout << "Connected client" << endl;

	return hClientSock;
}
