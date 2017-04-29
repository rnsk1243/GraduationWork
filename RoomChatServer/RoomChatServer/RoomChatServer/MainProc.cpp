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
	// EnterChannelNum 채널에 입장
	if (!commandController.getChannelHandler().enterChannel(&clientInfo, channelManager, EnterChannelNum))
	{
		return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
	}
	while (true)
	{
		int isRecvSuccesResultValue = actionNetWork.recvn(clientInfo, commandController);
		if (SUCCES_RECV == isRecvSuccesResultValue)// 메시지 받기 성공 일때 각 클라이언트에게 메시지 보냄
		{
			if (ERROR_SEND == actionNetWork.sendn(clientInfo, roomManager, channelManager))
			{
				if (commandController.deleteClientSocket(clientInfo)) // 채널 또는 방의 MyInfoList에서 제거한 후 성공하면
				{
					_endthreadex(0);
				}
			}
		}
		else if (ERROR_RECV == isRecvSuccesResultValue) // 메시지 받기 실패 소켓 해제
		{
			cout << "소켓 오류로 인하여 서버에서 나갔습니다." << endl;
			if (commandController.deleteClientSocket(clientInfo)) // 채널 또는 방의 MyInfoList에서 제거한 후 성공하면
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