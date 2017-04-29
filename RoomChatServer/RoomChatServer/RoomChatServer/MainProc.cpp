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
#include<thread>
#include"ErrorHandler.h"
using namespace std;


//thSendRecv(SOCKET& clientSocket, CCommandController& commandController, CActionNetWork& actionNetWork)

int thSendRecv(void* v_clientSocket, void* v_commandController, void* v_actionNetWork)
{
	SOCKET& clientSocket = (*(SOCKET*)v_clientSocket);
	CCommandController& commandController = (*(CCommandController*)v_commandController);
	CActionNetWork& actionNetWork = (*(CActionNetWork*)v_actionNetWork);

	CLobby lobby;
	int isLogin = 0;
	while (SUCCES_LOGIN != isLogin)
	{
		isLogin = lobby.ActionServiceLobby(clientSocket, actionNetWork);
		if (ERROR_RECV == isLogin || ERROR_SEND == isLogin)
		{
			_endthreadex(0);
		}
	}

	CLink clientInfo(clientSocket, lobby.getMessageStruct().message);
	CChannelManager& channelManager = commandController.getChannelManager();
	CRoomManager& roomManager = commandController.getRoomManager();
	// EnterChannelNum ä�ο� ����
	if (!commandController.getChannelHandler().enterChannel(&clientInfo, channelManager, EnterChannelNum))
	{
		return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
	}
	while (true)
	{
		int isRecvSuccesResultValue = actionNetWork.recvn(clientInfo, commandController);
		if (SUCCES_RECV == isRecvSuccesResultValue)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			if (ERROR_SEND == actionNetWork.sendn(clientInfo, roomManager, channelManager))
			{
				if (commandController.deleteClientSocket(clientInfo)) // ä�� �Ǵ� ���� MyInfoList���� ������ �� �����ϸ�
				{
					_endthreadex(0);
				}
			}
		}
		else if (ERROR_RECV == isRecvSuccesResultValue) // �޽��� �ޱ� ���� ���� ����
		{
			cout << "���� ������ ���Ͽ� �������� �������ϴ�." << endl;
			if (commandController.deleteClientSocket(clientInfo)) // ä�� �Ǵ� ���� MyInfoList���� ������ �� �����ϸ�
			{
				_endthreadex(0);
			}
			return 0;
		}
	}
}

void main()
{
	CReadyNetWork readyNetWork;
	CCommandController commandController;
	CActionNetWork actionNetWork;

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		readyNetWork.Accept(*clientSocket);
		
		thread clientThread(thSendRecv, clientSocket, &commandController, &actionNetWork);
		clientThread.detach();
	}
	
	getchar();
}