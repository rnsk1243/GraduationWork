#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;

const int Port = 9000;

class CReadyNetWork
{
	SOCKET* hServSock;
	CReadyNetWork(const CReadyNetWork&);
	CReadyNetWork&  operator=(const CReadyNetWork&);
public:
	CReadyNetWork();
	~CReadyNetWork();
	void Accept(SOCKET& hClientSock);
};

