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


const int MakeThreadNum = 3;
const int ChannelAmount = 5;

struct SendRecvParam
{
	SOCKET& clientSocket;
	CRoomManager& roomManager;
	CChannelManager& channelManager;
	CCommandController& commandController;
	CActionNetWork& actionNetWork;
	SendRecvParam(SOCKET& clientSocket_, CRoomManager& roomManager_, CChannelManager& channelManager_, CCommandController& commandController_, CActionNetWork& actionNetWork_) :
		clientSocket(clientSocket_), roomManager(roomManager_), channelManager(channelManager_), commandController(commandController_), actionNetWork(actionNetWork_) {}
};

unsigned int __stdcall thSendRecv(PVOID pvParam)
{
	SendRecvParam* SRParam = (SendRecvParam*)pvParam;
	SOCKET& clientSocket = SRParam->clientSocket;
	CRoomManager& roomManager = SRParam->roomManager;
	CChannelManager& channelManager = SRParam->channelManager;
	CCommandController& commandController = SRParam->commandController;
	CActionNetWork& actionNetWork = SRParam->actionNetWork;

	MessageStruct MS;
	CLink clientInfo(clientSocket, MS);
	// StartChannelNum ä�ο� ����
	commandController.getChannelHandler().enterChannel(&clientInfo, &channelManager, EnterChannelNum);

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
				_endthreadex(0);
			}
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
	CChannelManager channelManager(ChannelAmount);
	CRoomManager roomManager;
	CCommandController commandController(channelManager, roomManager, channelHandler, roomHandler);

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		readyNetWork.Accept(*clientSocket);
		SendRecvParam SRParam(*clientSocket, roomManager, channelManager, commandController, actionNetWork);
		_beginthreadex(NULL, NULL, thSendRecv, &SRParam, 0, NULL);
	}
	
	getchar();
}