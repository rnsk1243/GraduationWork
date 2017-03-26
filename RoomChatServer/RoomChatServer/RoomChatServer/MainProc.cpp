#include<iostream>
#include"MakeSocket.h"
#include"CommandController.h"
#include"Link.h"
#include"ChannelHandler.h"
#include"RoomHandler.h"
#include"RoomChannelManager.h"
#include"Channel.h"
#include"SendRecv.h"
#include<process.h>
using namespace std;

const int StartChannelNum = 0;
const int MakeThreadNum = 3;
const int ChannelNum = 5;

struct SendRecvParam
{
	CMakeSocket* socket;
	CRoomChannelManager* roomChannelManager;
	CCommandController* commandController;
	CSendRecv* sendRecv;
	SendRecvParam(CMakeSocket* socket_, CRoomChannelManager* roomChannelManager_, CCommandController* commandController_, CSendRecv* sendRecv_) :
		socket(socket_), roomChannelManager(roomChannelManager_), commandController(commandController_), sendRecv(sendRecv_) {}
};

unsigned int __stdcall thSendRecv(PVOID pvParam)
{
	SendRecvParam* SRParam = (SendRecvParam*)pvParam;
	CMakeSocket* socket = SRParam->socket;
	CRoomChannelManager* roomChannelManager = SRParam->roomChannelManager;
	CCommandController* commandController = SRParam->commandController;
	CSendRecv* sendRecv = SRParam->sendRecv;

	SOCKET* clientSocket = socket->Accept();
	CLink* clientInfo = new CLink(clientSocket);
	// StartChannelNum ä�ο� ����
	commandController->getChannelHandler()->enterChannel(clientInfo, roomChannelManager, StartChannelNum);

	while (true)
	{
		int isRecvSuccesResultValue = sendRecv->recvn(clientInfo, commandController);
		if (SuccesRecv == isRecvSuccesResultValue)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			// ���� �޽��� ���� �ӽ� ����
			//MessageStruct message(*clientInfo->getMessageStruct());

			sendRecv->sendn(clientInfo, roomChannelManager);
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
	CSendRecv* sendRecv = new CSendRecv();
	CMakeSocket* socket = new CMakeSocket();
	CChannelHandler* channelHandler = new CChannelHandler();
	CRoomHandler* roomHandler = new CRoomHandler();
	CRoomChannelManager* roomChannelManager = new CRoomChannelManager();
	CCommandController* commandController = new CCommandController(roomChannelManager, channelHandler, roomHandler);
	for (int i = ChannelNum; i >= 0; i--)
	{
		CChannel* newChannel = new CChannel(i);
		roomChannelManager->pushChannel(newChannel);
	}
	SendRecvParam* SRParam = new SendRecvParam(socket, roomChannelManager, commandController, sendRecv);

	for(int i=0; i<MakeThreadNum; i++)
	{
		_beginthreadex(NULL, NULL, thSendRecv, SRParam, 0, NULL);
	}
	getchar();
}