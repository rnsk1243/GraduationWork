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
	// 채널옮김 함수(옮기는 소켓, 옮기고싶은 채널 번호)
	bool enterChannel(CMyInfo* clientInfo, CLink* link, int targetChannelNo);
	// 채널 에서 나가기 (나가고자하는 소켓)
	bool exitChannel(CMyInfo* clientInfo, CLink* link);
	bool pushChannel(CChannel* newChannel);
};

