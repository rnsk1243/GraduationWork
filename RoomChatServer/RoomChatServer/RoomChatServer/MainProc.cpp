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
	//g_DataSize g_dataRecv;
	g_DataSize g_dataSize;
	g_dataSize.set_clientnum(SRParam->getClientNum());
	g_dataSize.set_size(6);
	g_dataSize.set_type(g_DataType::MESSAGE);
	CLobby lobby;

	CLink clientInfo(clientSocket, "우희", g_dataSize.clientnum());
	// 구별 번호 보내기
	actionNetWork.sendn(g_dataSize, clientSocket);
	

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
//	CLink clientInfo(clientSocket, "우희", g_data.clientnum());
	CChannelManager& channelManager = commandController.getChannelManager();
	CRoomManager& roomManager = commandController.getRoomManager();
	g_Message recvResultMessage; 
	recvResultMessage.set_message("null");
	// StartChannelNum 채널에 입장
	commandController.getChannelHandler().enterChannel(&clientInfo, channelManager, EnterChannelNum);
	actionNetWork.sendn(g_dataSize, clientInfo, roomManager, channelManager); // 내가 생성되었다고 전원에게 알림
	while (true)
	{
		//g_dataRecv.Clear();
		int isRecvSuccesResultValue = actionNetWork.recvnData(clientInfo, g_dataSize, commandController, recvResultMessage);
		if (SUCCES_RECV == isRecvSuccesResultValue)
		{
			if(ERROR_SEND == actionNetWork.sendn(g_dataSize, clientInfo, roomManager, channelManager))
			{
				CErrorHandler::ErrorHandler(ERROR_SEND);
			}
		}
		else if (SUCCES_RECV_EVERY_SEND == isRecvSuccesResultValue)
		{
			if (ERROR_SEND == actionNetWork.sendn(g_dataSize, clientInfo, roomManager, channelManager, true))
			{
				CErrorHandler::ErrorHandler(ERROR_SEND);
			}
		}
		else if (ERROR_RECV == isRecvSuccesResultValue) // 메시지 받기 실패 소켓 해제
		{
			cout << "소켓 오류로 인하여 서버에서 나갔습니다." << endl;
			if (commandController.deleteClientSocket(clientInfo)) // 채널 또는 방의 MyInfoList에서 제거한 후 성공하면
			{
				_endthreadex(0);
			}
			// 스레드 반환
			return 0;
		}
	}
}

void main()
{
	CActionNetWork actionNetWork;
	CReadyNetWork readyNetWork;
	CCommandController commandController;

	int clientNum = 0; // 구별 번호.

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		readyNetWork.Accept(*clientSocket);
		SendRecvParam SRParam(*clientSocket, commandController, actionNetWork, clientNum);
		_beginthreadex(NULL, NULL, thSendRecv, &SRParam, 0, NULL);
		clientNum++; // 구별번호 증가
	}
	
	getchar();
}