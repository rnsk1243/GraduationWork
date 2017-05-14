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
	int clientNum;
	SendRecvParam(SOCKET& clientSocket_, CCommandController& commandController_, CActionNetWork& actionNetWork_, int clientNum_) :
		clientSocket(clientSocket_), commandController(commandController_), actionNetWork(actionNetWork_), clientNum(clientNum_) {}
	int getClientNum() { return clientNum; }
};

unsigned int __stdcall thSendRecv(PVOID pvParam)
{
	SendRecvParam* SRParam = (SendRecvParam*)pvParam;
	SOCKET& clientSocket = SRParam->clientSocket;
	CCommandController& commandController = SRParam->commandController;
	CActionNetWork& actionNetWork = SRParam->actionNetWork;
	g_DataSize g_data;
	g_data.set_clientnum(SRParam->getClientNum());
	g_data.set_size(0);
	g_data.set_type(g_DataType::MESSAGE);
	CLobby lobby;

	// ���� ��ȣ ������
	actionNetWork.sendn(g_data, clientSocket);

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
	CLink clientInfo(clientSocket, "����", g_data.clientnum());
	CChannelManager& channelManager = commandController.getChannelManager();
	CRoomManager& roomManager = commandController.getRoomManager();
	// StartChannelNum ä�ο� ����
	commandController.getChannelHandler().enterChannel(&clientInfo, channelManager, EnterChannelNum);
	actionNetWork.sendn(g_data, clientInfo, roomManager, channelManager); // ���� �����Ǿ��ٰ� �������� �˸�
	while (true)
	{
		int isRecvSuccesResultValue = actionNetWork.recvnData(clientInfo, g_data);
		if (SUCCES_RECV == isRecvSuccesResultValue)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			if (ERROR_SEND == actionNetWork.sendn(g_data, clientInfo, roomManager, channelManager))
			{
				CErrorHandler::ErrorHandler(ERROR_SEND);
			}
		}
		else if (ERROR_RECV == isRecvSuccesResultValue) // �޽��� �ޱ� ���� ���� ����
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
	CCommandController commandController;

	int clientNum = 0; // ���� ��ȣ.

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		readyNetWork.Accept(*clientSocket);
		SendRecvParam SRParam(*clientSocket, commandController, actionNetWork, clientNum);
		_beginthreadex(NULL, NULL, thSendRecv, &SRParam, 0, NULL);
		clientNum++; // ������ȣ ����
	}
	
	getchar();
}