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
#include"Listener.h"
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


int thSendRecv(void* v_clientSocket)
{
	const SOCKET* clientSocket = ((SOCKET*)v_clientSocket);
	CGuestLink guest(clientSocket);
	int isLogin = 0;
	vector<string> userInfo;
	while (SUCCES_LOGIN != isLogin)
	{
		isLogin = LobbyStatic->ActionServiceLobby(&guest, userInfo);
		if (ERROR_NULL_LINK_RECV == isLogin || ERROR_NULL_LINK_SEND == isLogin)
		{
			cout << "종료" << endl;
			closesocket(*clientSocket);
			delete clientSocket;
			return ErrorHandStatic->ErrorHandler(EnumErrorCode(isLogin));
			//_endthreadex(0);
		}
		ErrorHandStatic->ErrorHandler(EnumErrorCode(isLogin));
	}
	LinkPtr shared_clientInfo(new CLink(clientSocket, userInfo[IndexUserPK], userInfo[IndexUserID].c_str()));

	// EnterChannelNum 채널에 입장
	vector<string> commandChannel;
	CommandControllerStatic->SetEnterChannel(shared_clientInfo, StartEnterChannelNum);

	if (false == ReadHandlerStatic->ReadUserCard(shared_clientInfo, NameMemberCardInfoTxt))
		return 0;
	if (false == ReadHandlerStatic->ReadUserGoods(shared_clientInfo, NameMemberGoodsTxt))
		return 0;
	cout << "========== 보유 카드 =============" << endl;

	MyCardVectorIt cardBegin = shared_clientInfo->GetIterMyCardBegin();
	MyCardVectorIt cardEnd = shared_clientInfo->GetIterMyCardEnd();
	for (; cardBegin != cardEnd; ++cardBegin)
	{
		cout << (*cardBegin).get()->GetCardName() << endl;
		cout << (*cardBegin).get()->GetAmount() << endl;
	}
	cout << "==================================" << endl;
	cout << "보유 재화 = " << shared_clientInfo->GetMyMoney() << endl;
	shared_clientInfo.get()->SendnMine("로그인 성공");
	ErrorHandStatic->ErrorHandler(SUCCES_LOGIN, shared_clientInfo);
	while (true)
	{
		string recvMessage;
		int isRecvSuccesResultValue = ListenerStatic->Recvn(shared_clientInfo.get()->GetClientSocket(), recvMessage);
		if (SUCCES_RECV == isRecvSuccesResultValue)// 메시지 받기 성공 일때 각 클라이언트에게 메시지 보냄
		{
			vector<string> commandMessage = ReadHandlerStatic->Parse(recvMessage, '/');
			CommandControllerStatic->CommandHandling(shared_clientInfo, commandMessage);
		}
		else if (ERROR_RECV == isRecvSuccesResultValue || ERROR_NULL_LINK_RECV == isRecvSuccesResultValue) // 메시지 받기 실패 소켓 해제
		{
			cout << "소켓 오류로 인하여 서버에서 나갔습니다." << endl;
			CommandControllerStatic->DeleteClientSocket(shared_clientInfo);
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
	

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		ReadyNetWorkStatic->Accept(clientSocket);
		
		thread clientThread(thSendRecv, clientSocket);
		clientThread.detach();
	}
	getchar();
}