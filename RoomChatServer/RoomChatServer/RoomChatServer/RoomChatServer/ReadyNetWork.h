#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;

class CReadyNetWork
{
	SOCKET* mServSock;
public:
	CReadyNetWork();
	CReadyNetWork(const CReadyNetWork&) = delete;
	CReadyNetWork&  operator=(const CReadyNetWork&) = delete;
	~CReadyNetWork();
	void Accept(SOCKET& hClientSock);
};

