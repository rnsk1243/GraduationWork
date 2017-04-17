#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;



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

