#include<iostream>
#include"ReadyNetWork.h"
#include"CommandController.h"
#include"Link.h"
#include"ChannelHandler.h"
#include"RoomHandler.h"
#include"RoomManager.h"
#include"ChannelManager.h"
#include"Channel.h"
#include"ActionNetWork.h"
#include<process.h>
using namespace std;

const int StartChannelNum = 0;
const int MakeThreadNum = 3;
const int ChannelNum = 5;

int PK = 0;

struct SendRecvParam
{
	SOCKET& clientSocket;
	CRoomManager& roomManager;
	CChannelManager& channelManager;
	CCommandController& commandController;
	CActionNetWork& actionNetWork;
	MessageStruct& MS;
	int PK;
	SendRecvParam(SOCKET& clientSocket_, CRoomManager& roomManager_, CChannelManager& channelManager_, CCommandController& commandController_, CActionNetWork& actionNetWork_, MessageStruct& MS_, int pk_) :
		clientSocket(clientSocket_), roomManager(roomManager_), channelManager(channelManager_), commandController(commandController_), actionNetWork(actionNetWork_), MS(MS_), PK(pk_) {}
};

unsigned int __stdcall thSendRecv(PVOID pvParam)
{
	SendRecvParam* SRParam = (SendRecvParam*)pvParam;
	SOCKET& clientSocket = SRParam->clientSocket;
	CRoomManager& roomManager = SRParam->roomManager;
	CChannelManager& channelManager = SRParam->channelManager;
	CCommandController& commandController = SRParam->commandController;
	CActionNetWork& actionNetWork = SRParam->actionNetWork;
	MessageStruct& MS = SRParam->MS;
	int PK = SRParam->PK;


	CLink clientInfo(clientSocket, MS, PK);
	// StartChannelNum ä�ο� ����
	commandController.getChannelHandler().enterChannel(clientInfo, channelManager, StartChannelNum);

	while (true)
	{
		int isRecvSuccesResultValue = actionNetWork.recvn(clientInfo, commandController);
		if (SuccesRecv == isRecvSuccesResultValue)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			// ���� �޽��� ���� �ӽ� ����
			//MessageStruct message(*clientInfo->getMessageStruct());

			actionNetWork.sendn(clientInfo, roomManager, channelManager);
		}
		else if (OccuredError == isRecvSuccesResultValue) // �޽��� �ޱ� ���� ���� ����
		{
			cout << "���� ������ ���Ͽ� �������� �������ϴ�." << endl;
			if (commandController.deleteClientSocket(clientInfo)) // ä�� �Ǵ� ���� MyInfoList���� ������ �� �����ϸ�
			{
			}
			_endthreadex(0);
			// ������ ��ȯ
			return 0;
		}
	}
}

void main()
{
	CActionNetWork actionNetWork;
	CReadyNetWork readyNetWork;
	CChannelHandler channelHandler;
	CRoomHandler roomHandler;
	CChannelManager channelManager;
	CRoomManager roomManager;
	CCommandController commandController(channelManager, roomManager, channelHandler, roomHandler);
	for (int i = ChannelNum; i >= 0; i--)
	{
		CChannel newChannel(i);
		channelManager.pushChannel(newChannel);
	}

	while (true)
	{
		SOCKET clientSocket = readyNetWork.Accept();
		if (clientSocket != -1)
		{
			MessageStruct MS;
			SendRecvParam SRParam(clientSocket, roomManager, channelManager, commandController, actionNetWork, MS, PK);
			_beginthreadex(NULL, NULL, thSendRecv, &SRParam, 0, NULL);
			PK++;
		}
	}
	
	getchar();
}