//#pragma comment(lib,"version.lib")
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
#include<process.h>
#include<thread>
#include"ErrorHandler.h"
#include"resource.h"
#include"BasicExcel.hpp"
#include<Windows.h>
#include"MinidumpHelp.h"
#include"ReadHandler.h"
#include"ConstEnumInfo.h"
using namespace std;
using namespace YExcel;

void printVersionInfo()
{
	// 버전정보를 담을 버퍼
	char* buffer = NULL;
	// 버전을 확인할 파일
	char* name = "RoomChatServer.exe";

	DWORD infoSize = 0;

	// 파일로부터 버전정보데이터의 크기가 얼마인지를 구합니다.
	infoSize = GetFileVersionInfoSize(name, 0);
	if (infoSize == 0) return;

	// 버퍼할당
	buffer = new char[infoSize];
	if (buffer)
	{
		// 버전정보데이터를 가져옵니다.
		if (GetFileVersionInfo(name, 0, infoSize, buffer) != 0)
		{
			VS_FIXEDFILEINFO* pFineInfo = NULL;
			UINT bufLen = 0;
			// buffer로 부터 VS_FIXEDFILEINFO 정보를 가져옵니다.
			if (VerQueryValue(buffer, "\\", (LPVOID*)&pFineInfo, &bufLen) != 0)
			{
				WORD majorVer, minorVer, buildNum, revisionNum;
				majorVer = HIWORD(pFineInfo->dwFileVersionMS);
				minorVer = LOWORD(pFineInfo->dwFileVersionMS);
				buildNum = HIWORD(pFineInfo->dwFileVersionLS);
				revisionNum = LOWORD(pFineInfo->dwFileVersionLS);

				// 파일버전 출력
				printf("version : %d,%d,%d,%d\n", majorVer, minorVer, buildNum, revisionNum);
			}
		}
		delete[] buffer;
	}
}

//thSendRecv(SOCKET& clientSocket, CCommandController& commandController, CActionNetWork& actionNetWork)

int thSendRecv(void* v_clientSocket, void* v_commandController, void* v_actionNetWork, void* v_lobby)
{
	SOCKET& clientSocket = (*(SOCKET*)v_clientSocket);
	CCommandController& commandController = (*(CCommandController*)v_commandController);
	CActionNetWork& actionNetWork = (*(CActionNetWork*)v_actionNetWork);
	CLobby& lobby = (*(CLobby*)v_lobby);
	//CLobby lobby;
	int isLogin = 0;
	vector<string> userInfo;
	while (SUCCES_LOGIN != isLogin)
	{
		isLogin = lobby.ActionServiceLobby(clientSocket, actionNetWork, userInfo);
		CErrorHandler::ErrorHandler(EnumErrorCode(isLogin));
		if (ERROR_RECV == isLogin || ERROR_SEND == isLogin)
		{
			return CErrorHandler::ErrorHandler(EnumErrorCode(isLogin));
			//_endthreadex(0);
		}
	}
	shared_ptr<CLink> shared_clientInfo(new CLink(clientSocket, userInfo[IndexUserPK], lobby.getMessageStruct().message));
	CChannelManager& channelManager = commandController.GetChannelManager();
	CRoomManager& roomManager = commandController.GetRoomManager();
	// EnterChannelNum 채널에 입장
	if (!commandController.GetChannelHandler().MoveNextChannel(shared_clientInfo, channelManager, EnterChannelNum))
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

	ReadHandlerStatic->ReadUserCard(clientInfo, NameMemberCardInfoTxt);

	cout << "========== 보유 카드 =============" << endl;

	MyCardVectorIt cardBegin = clientInfo->GetIterMyCardBegin();
	MyCardVectorIt cardEnd = clientInfo->GetIterMyCardEnd();
	for (; cardBegin != cardEnd; ++cardBegin)
	{
		cout << (*cardBegin).get()->getCardName() << endl;
		cout << (*cardBegin).get()->getAmount() << endl;
	}
	cout << "==================================" << endl;

	while (true)
	{
		int isRecvSuccesResultValue = actionNetWork.Recvn(shared_clientInfo, commandController);
		if (SUCCES_RECV == isRecvSuccesResultValue)// 메시지 받기 성공 일때 각 클라이언트에게 메시지 보냄
		{
			if (ERROR_SEND == actionNetWork.Sendn(*clientInfo, roomManager, channelManager))
			{
				if (!commandController.DeleteClientSocket(*clientInfo)) // 채널 또는 방의 MyInfoList에서 제거한 후 성공하면
				{
					return CErrorHandler::ErrorHandler(ERROR_DELETE_SOCKET);
					//_endthreadex(0);
				}
			}
		}
		else if (ERROR_RECV == isRecvSuccesResultValue) // 메시지 받기 실패 소켓 해제
		{
			cout << "소켓 오류로 인하여 서버에서 나갔습니다." << endl;
			if (!commandController.DeleteClientSocket(*clientInfo)) // 채널 또는 방의 MyInfoList에서 제거한 후 성공하면
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
	//MinidumpHelp dump;
	//dump.install_self_mini_dump();
	/////////// 버전 정보 출력 ///////////
	printVersionInfo();
	//////////////////////////////////////
	BasicExcel excel;
	excel.ReadExcel("cardInfo.xls");

	CReadyNetWork readyNetWork;
	CCommandController commandController;
	CActionNetWork actionNetWork;
	string strNextUserNum = ReadHandlerStatic->GetNextUserNum(MakeNextJoinNumberTxt);
	int nextUserNum = stoi(strNextUserNum);
	CLobby lobby(nextUserNum);

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		readyNetWork.Accept(*clientSocket);
		
		thread clientThread(thSendRecv, clientSocket, &commandController, &actionNetWork, &lobby);
		clientThread.detach();
	}
	
	getchar();
}