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
	// ���������� ���� ����
	char* buffer = NULL;
	// ������ Ȯ���� ����
	char* name = "RoomChatServer.exe";

	DWORD infoSize = 0;

	// ���Ϸκ��� ���������������� ũ�Ⱑ �������� ���մϴ�.
	infoSize = GetFileVersionInfoSize(name, 0);
	if (infoSize == 0) return;

	// �����Ҵ�
	buffer = new char[infoSize];
	if (buffer)
	{
		// �������������͸� �����ɴϴ�.
		if (GetFileVersionInfo(name, 0, infoSize, buffer) != 0)
		{
			VS_FIXEDFILEINFO* pFineInfo = NULL;
			UINT bufLen = 0;
			// buffer�� ���� VS_FIXEDFILEINFO ������ �����ɴϴ�.
			if (VerQueryValue(buffer, "\\", (LPVOID*)&pFineInfo, &bufLen) != 0)
			{
				WORD majorVer, minorVer, buildNum, revisionNum;
				majorVer = HIWORD(pFineInfo->dwFileVersionMS);
				minorVer = LOWORD(pFineInfo->dwFileVersionMS);
				buildNum = HIWORD(pFineInfo->dwFileVersionLS);
				revisionNum = LOWORD(pFineInfo->dwFileVersionLS);

				// ���Ϲ��� ���
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
	// EnterChannelNum ä�ο� ����
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

	cout << "========== ���� ī�� =============" << endl;

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
		if (SUCCES_RECV == isRecvSuccesResultValue)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			if (ERROR_SEND == actionNetWork.Sendn(*clientInfo, roomManager, channelManager))
			{
				if (!commandController.DeleteClientSocket(*clientInfo)) // ä�� �Ǵ� ���� MyInfoList���� ������ �� �����ϸ�
				{
					return CErrorHandler::ErrorHandler(ERROR_DELETE_SOCKET);
					//_endthreadex(0);
				}
			}
		}
		else if (ERROR_RECV == isRecvSuccesResultValue) // �޽��� �ޱ� ���� ���� ����
		{
			cout << "���� ������ ���Ͽ� �������� �������ϴ�." << endl;
			if (!commandController.DeleteClientSocket(*clientInfo)) // ä�� �Ǵ� ���� MyInfoList���� ������ �� �����ϸ�
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
	/////////// ���� ���� ��� ///////////
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