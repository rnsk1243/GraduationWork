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

struct SendRecvParam
{
	SOCKET* clientSocket;
	CRoomManager* roomManager;
	CChannelManager* channelManager;
	CCommandController* commandController;
	CActionNetWork* actionNetWork;
	SendRecvParam(SOCKET* clientSocket_, CRoomManager* roomManager_, CChannelManager* channelManager_, CCommandController* commandController_, CActionNetWork* actionNetWork_) :
		clientSocket(clientSocket_), roomManager(roomManager_), channelManager(channelManager_), commandController(commandController_), actionNetWork(actionNetWork_) {}
};

unsigned int __stdcall thSendRecv(PVOID pvParam)
{
	SendRecvParam* SRParam = (SendRecvParam*)pvParam;
	SOCKET* clientSocket = SRParam->clientSocket;
	CRoomManager* roomManager = SRParam->roomManager;
	CChannelManager* channelManager = SRParam->channelManager;
	CCommandController* commandController = SRParam->commandController;
	CActionNetWork* actionNetWork = SRParam->actionNetWork;


	CLink* clientInfo = new CLink(clientSocket);
	// StartChannelNum ä�ο� ����
	commandController->getChannelHandler()->enterChannel(clientInfo, channelManager, StartChannelNum);

	while (true)
	{
		int isRecvSuccesResultValue = actionNetWork->recvn(clientInfo, commandController);
		if (SuccesRecv == isRecvSuccesResultValue)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			// ���� �޽��� ���� �ӽ� ����
			//MessageStruct message(*clientInfo->getMessageStruct());

			actionNetWork->sendn(clientInfo, roomManager, channelManager);
		}
		else if (OccuredError == isRecvSuccesResultValue) // �޽��� �ޱ� ���� ���� ����
		{
			cout << "���� ������ ���Ͽ� �������� �������ϴ�." << endl;
			if (commandController->deleteClientSocket(clientInfo)) // ä�� �Ǵ� ���� MyInfoList���� ������ �� �����ϸ�
			{
				delete clientInfo;
			}
			_endthreadex(0);
			// ������ ��ȯ
			return 0;
		}
	}
}

void main()
{
	CActionNetWork* actionNetWork = new CActionNetWork();
	CReadyNetWork* readyNetWork = new CReadyNetWork();
	CChannelHandler* channelHandler = new CChannelHandler();
	CRoomHandler* roomHandler = new CRoomHandler();
	CChannelManager* channelManager = new CChannelManager();
	CRoomManager* roomManager = new CRoomManager();
	CCommandController* commandController = new CCommandController(channelManager, roomManager, channelHandler, roomHandler);
	for (int i = ChannelNum; i >= 0; i--)
	{
		CChannel* newChannel = new CChannel(i);
		channelManager->pushChannel(newChannel);
	}

	while (true)
	{
		SOCKET* clientSocket = readyNetWork->Accept();
		SendRecvParam* SRParam = new SendRecvParam(clientSocket, roomManager, channelManager, commandController, actionNetWork);
		_beginthreadex(NULL, NULL, thSendRecv, SRParam, 0, NULL);
	}
	
	getchar();
}