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
#include"Lobby.h"
#include"ConstEnumInfo.h"
#include<process.h>
using namespace std;



struct SendRecvParam
{
	SOCKET& clientSocket;
	CCommandController& commandController;
	CActionNetWork& actionNetWork;
	SendRecvParam(SOCKET& clientSocket_, CCommandController& commandController_, CActionNetWork& actionNetWork_) :
		clientSocket(clientSocket_), commandController(commandController_), actionNetWork(actionNetWork_) {}
};

unsigned int __stdcall thSendRecv(PVOID pvParam)
{
	SendRecvParam* SRParam = (SendRecvParam*)pvParam;
	SOCKET& clientSocket = SRParam->clientSocket;
	CCommandController& commandController = SRParam->commandController;
	CActionNetWork& actionNetWork = SRParam->actionNetWork;

	CLobby lobby;
	/*bool isLogin = false;
	while (!isLogin)
	{
		lobby.SendMenuInfo(clientSocket, actionNetWork);
		actionNetWork.recvn(clientSocket, lobby.getMessageStruct());
		int choose = lobby.ChooseMenu(lobby.getMessageStruct().message(), clientSocket, actionNetWork);
		switch (choose)
		{
		case 1:
			if (lobby.Login(clientSocket, actionNetWork))
			{
				isLogin = true;
				break;
			}
			else
			{
				break;
			}
		case 2:
			lobby.JoinMember(clientSocket, actionNetWork);
			break;
		default:
			break;
		}
	}*/

	//CLink clientInfo(clientSocket, lobby.getMessageStruct().message());
	CLink clientInfo(clientSocket, "����");
	CChannelManager& channelManager = commandController.getChannelManager();
	CRoomManager& roomManager = commandController.getRoomManager();
	// StartChannelNum ä�ο� ����
	commandController.getChannelHandler().enterChannel(&clientInfo, channelManager, EnterChannelNum);

	while (true)
	{
		int isRecvSuccesResultValue = actionNetWork.recvn(clientInfo, commandController);
		if (SOCKET_ERROR == isRecvSuccesResultValue)
			return 0;
		//if (SuccesRecv == isRecvSuccesResultValue)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		//{
		//	// ���� �޽��� ���� �ӽ� ����
		//	//MessageStruct message(*clientInfo->getMessageStruct());

		//	actionNetWork.sendn(clientInfo, roomManager, channelManager);
		//}
		//else if (OccuredError == isRecvSuccesResultValue) // �޽��� �ޱ� ���� ���� ����
		//{
		//	cout << "���� ������ ���Ͽ� �������� �������ϴ�." << endl;
		//	if (commandController.deleteClientSocket(clientInfo)) // ä�� �Ǵ� ���� MyInfoList���� ������ �� �����ϸ�
		//	{
		//		_endthreadex(0);
		//	}
		//	// ������ ��ȯ
		//	return 0;
		//}
	}
}

void main()
{
	CActionNetWork actionNetWork;
	CReadyNetWork readyNetWork;
	CCommandController commandController;

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		readyNetWork.Accept(*clientSocket);
		SendRecvParam SRParam(*clientSocket, commandController, actionNetWork);
		_beginthreadex(NULL, NULL, thSendRecv, &SRParam, 0, NULL);
	}
	
	getchar();
}