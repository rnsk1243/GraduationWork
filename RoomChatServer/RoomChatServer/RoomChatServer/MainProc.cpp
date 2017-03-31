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
	// StartChannelNum 채널에 입장
	commandController.getChannelHandler().enterChannel(clientInfo, channelManager, StartChannelNum);

	while (true)
	{
		int isRecvSuccesResultValue = actionNetWork.recvn(clientInfo, commandController);
		if (SuccesRecv == isRecvSuccesResultValue)// 메시지 받기 성공 일때 각 클라이언트에게 메시지 보냄
		{
			// 받은 메시지 내용 임시 복사
			//MessageStruct message(*clientInfo->getMessageStruct());

			actionNetWork.sendn(clientInfo, roomManager, channelManager);
		}
		else if (OccuredError == isRecvSuccesResultValue) // 메시지 받기 실패 소켓 해제
		{
			cout << "소켓 오류로 인하여 서버에서 나갔습니다." << endl;
			if (commandController.deleteClientSocket(clientInfo)) // 채널 또는 방의 MyInfoList에서 제거한 후 성공하면
			{
			}
			_endthreadex(0);
			// 스레드 반환
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