#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;

class CReadyNetWork
{
	SOCKET* mServSock;
	CReadyNetWork();
public:
	static CReadyNetWork* GetInstance();
	CReadyNetWork(const CReadyNetWork&) = delete;
	CReadyNetWork&  operator=(const CReadyNetWork&) = delete;
	~CReadyNetWork();
	void Accept(SOCKET * hClientSock);
};
static CReadyNetWork * ReadyNetWorkStatic = CReadyNetWork::GetInstance();
