#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;

const int Port = 9000;

class CMakeSocket
{
	SOCKET* hServSock;
public:
	CMakeSocket();
	~CMakeSocket();
	SOCKET* Accept();
};

