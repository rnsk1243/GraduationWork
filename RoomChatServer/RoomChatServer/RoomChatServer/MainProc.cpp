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

int thSendRecv(void* v_clientSocket, void* v_commandController, void* v_actionNetWork, void* v_lobby)
{
	SOCKET& clientSocket = (*(SOCKET*)v_clientSocket);
	CCommandController& commandController = (*(CCommandController*)v_commandController);
	CActionNetWork& actionNetWork = (*(CActionNetWork*)v_actionNetWork);
	CLobby& lobby = (*(CLobby*)v_lobby);
	//CLobby lobby;
	int isLogin = 0;
	while (SUCCES_LOGIN != isLogin)
	{
		isLogin = lobby.ActionServiceLobby(clientSocket, actionNetWork);
		CErrorHandler::ErrorHandler(EnumErrorCode(isLogin));
		if (ERROR_RECV == isLogin || ERROR_SEND == isLogin)
		{
			return CErrorHandler::ErrorHandler(EnumErrorCode(isLogin));
			//_endthreadex(0);
		}
	}
	shared_ptr<CLink> shared_clientInfo(new CLink(clientSocket, lobby.getMessageStruct().message));
	CChannelManager& channelManager = commandController.getChannelManager();
	CRoomManager& roomManager = commandController.getRoomManager();
	// EnterChannelNum 채널에 입장
	if (!commandController.getChannelHandler().enterChannel(shared_clientInfo, channelManager, EnterChannelNum))
	{
		return CErrorHandler::ErrorHandler(ERROR_ENTER_CHANNEL);
	}
	CLink* clientInfo = nullptr;
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
	}
	else
	{
		return CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
	}
	while (true)
	{
		int isRecvSuccesResultValue = actionNetWork.recvn(shared_clientInfo, commandController);
		if (SUCCES_RECV == isRecvSuccesResultValue)// 메시지 받기 성공 일때 각 클라이언트에게 메시지 보냄
		{
			if (ERROR_SEND == actionNetWork.sendn(*clientInfo, roomManager, channelManager))
			{
				if (!commandController.deleteClientSocket(*clientInfo)) // 채널 또는 방의 MyInfoList에서 제거한 후 성공하면
				{
					return CErrorHandler::ErrorHandler(ERROR_DELETE_SOCKET);
					//_endthreadex(0);
				}
			}
		}
		else if (ERROR_RECV == isRecvSuccesResultValue) // 메시지 받기 실패 소켓 해제
		{
			cout << "소켓 오류로 인하여 서버에서 나갔습니다." << endl;
			if (!commandController.deleteClientSocket(*clientInfo)) // 채널 또는 방의 MyInfoList에서 제거한 후 성공하면
			{
				return CErrorHandler::ErrorHandler(ERROR_DELETE_SOCKET);
				//_endthreadex(0);
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
	CLobby lobby;

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		readyNetWork.Accept(*clientSocket);
		
		thread clientThread(thSendRecv, clientSocket, &commandController, &actionNetWork, &lobby);
		clientThread.detach();
	}
	
	getchar();
}