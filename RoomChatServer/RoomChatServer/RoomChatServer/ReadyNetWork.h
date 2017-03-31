#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;

const int Port = 9000;

class CReadyNetWork
{
	SOCKET* hServSock;
public:
	CReadyNetWork();
	~CReadyNetWork();
	SOCKET Accept();
};

