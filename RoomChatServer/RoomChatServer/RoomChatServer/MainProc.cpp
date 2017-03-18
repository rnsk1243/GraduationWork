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
		if (SuccesRecv == isRecvSuccesResultValue)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			netWork->sendn();
		}
		else if (OccuredError == isRecvSuccesResultValue) // �޽��� �ޱ� ���� ���� ����
		{
			cout << "���� ������ ���Ͽ� �������� �������ϴ�." << endl;
			delete netWork;
			_endthreadex(0);
			// ������ ��ȯ
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
		// startä�ο� ����
		channelHandlere->enterChannel(myInfo, link, StartChannelNum);
		_beginthreadex(NULL, NULL, thNetWork, netWork, 0, NULL);
	}
}