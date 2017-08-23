#pragma once
#include<WinSock2.h>
#include<iostream>
#include"ConstEnumInfo.h"
using namespace std;

class CGuestLink
{
	const SOCKET& mClientSocket;
public:
	CGuestLink(const SOCKET & clientSocket);
	~CGuestLink();
	const SOCKET GetClientSocket();
	void Sendn(const string& message, int flags = 0);
};