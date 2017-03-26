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
	// StartChannelNum 채널에 입장
	commandController->getChannelHandler()->enterChannel(clientInfo, roomChannelManager, StartChannelNum);

	while (true)
	{
		int isRecvSuccesResultValue = sendRecv->recvn(clientInfo, commandController);
		if (SuccesRecv == isRecvSuccesResultValue)// 메시지 받기 성공 일때 각 클라이언트에게 메시지 보냄
		{
			// 받은 메시지 내용 임시 복사
			//MessageStruct message(*clientInfo->getMessageStruct());

			sendRecv->sendn(clientInfo, roomChannelManager);
		}
		else if (OccuredError == isRecvSuccesResultValue) // 메시지 받기 실패 소켓 해제
		{
			cout << "소켓 오류로 인하여 서버에서 나갔습니다." << endl;
			if (commandController->deleteClientSocket(clientInfo)) // 채널 또는 방의 MyInfoList에서 제거한 후 성공하면
			{
				delete clientInfo;
			}
			_endthreadex(0);
			// 스레드 반환
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