#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"Channel.h"
#include"MyInfo.h"


class CChannelHandlere
{
public:
	CChannelHandlere();
	~CChannelHandlere();
	// ä�οű� �Լ�(�ű�� ����, �ű����� ä�� ��ȣ)
	bool enterChannel(CMyInfo* clientInfo, CLink* link, int targetChannelNo);
	// ä�� ���� ������ (���������ϴ� ����)
	bool exitChannel(CMyInfo* clientInfo, CLink* link);
	bool pushChannel(CChannel* newChannel);
};

