#include<iostream>
#include"MakeSocket.h"
#include"NetWork.h"
#include"Link.h"
#include"ChannelHandlere.h"
#include"RoomHandlere.h"
#include"MyInfo.h"
#include"Channel.h"
#include<process.h>
using namespace std;

const int StartChannelNum = 0;

unsigned int __stdcall thNetWork(PVOID pvParam)
{
	CNetWork* netWork = (CNetWork*)pvParam;

	while (true)
	{
		int isRecvSuccesResultValue = netWork->recvn();
		if (SuccesRecv == isRecvSuccesResultValue)// 메시지 받기 성공 일때 각 클라이언트에게 메시지 보냄
		{
			netWork->sendn();
		}
		else if (OccuredError == isRecvSuccesResultValue) // 메시지 받기 실패 소켓 해제
		{
			cout << "소켓 오류로 인하여 서버에서 나갔습니다." << endl;
			delete netWork;
			_endthreadex(0);
			// 스레드 반환
			return 0;
		}
	}
}

void main()
{
	CMakeSocket* socket = new CMakeSocket();
	CChannelHandlere* channelHandlere = new CChannelHandlere();
	CRoomHandlere* roomHandlere = new CRoomHandlere();
	CLink* link = new CLink();
	for (int i = 5; i >= 0; i--)
	{
		CChannel* newChannel = new CChannel(i);
		link->pushChannel(newChannel);
	}
	while (true)
	{
		SOCKET* clientSocket = socket->Accept();
		CMyInfo* myInfo = new CMyInfo(clientSocket);
		CNetWork* netWork = new CNetWork(link, myInfo, channelHandlere, roomHandlere);
		// start채널에 입장
		channelHandlere->enterChannel(myInfo, link, StartChannelNum);
		_beginthreadex(NULL, NULL, thNetWork, netWork, 0, NULL);
	}
}