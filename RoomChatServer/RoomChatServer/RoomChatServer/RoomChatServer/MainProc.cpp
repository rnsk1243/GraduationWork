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

int thSendRecv(void* v_clientSocket, void* v_commandController, void* v_listener, void* v_lobby)
{
	SOCKET& clientSocket = (*(SOCKET*)v_clientSocket);
	CCommandController& commandController = (*(CCommandController*)v_commandController);
	CListener& listener = (*(CListener*)v_listener);
	CLobby& lobby = (*(CLobby*)v_lobby);
	CGuestLink guest(clientSocket);
	//CLobby lobby;
	int isLogin = 0;
	vector<string> userInfo;
	while (SUCCES_LOGIN != isLogin)
	{
		isLogin = lobby.ActionServiceLobby(guest, listener, userInfo);
		if (ERROR_NULL_LINK_RECV == isLogin || ERROR_NULL_LINK_SEND == isLogin)
		{
			cout << "����" << endl;
			closesocket(clientSocket);
			delete &clientSocket;
			return ErrorHandStatic->ErrorHandler(EnumErrorCode(isLogin));
			//_endthreadex(0);
		}
		ErrorHandStatic->ErrorHandler(EnumErrorCode(isLogin));
	}
	LinkPtr shared_clientInfo(new CLink(clientSocket, userInfo[IndexUserPK], userInfo[IndexUserID].c_str()));

	// EnterChannelNum ä�ο� ����
	vector<string> commandChannel;
	commandChannel.push_back(CommandChannal);
	commandChannel.push_back("1");
	commandController.CommandHandling(shared_clientInfo, commandChannel);

	if (false == ReadHandlerStatic->ReadUserCard(shared_clientInfo, NameMemberCardInfoTxt))
		return 0;
	if (false == ReadHandlerStatic->ReadUserGoods(shared_clientInfo, NameMemberGoodsTxt))
		return 0;
	cout << "========== ���� ī�� =============" << endl;

	MyCardVectorIt cardBegin = shared_clientInfo->GetIterMyCardBegin();
	MyCardVectorIt cardEnd = shared_clientInfo->GetIterMyCardEnd();
	for (; cardBegin != cardEnd; ++cardBegin)
	{
		cout << (*cardBegin).get()->GetCardName() << endl;
		cout << (*cardBegin).get()->GetAmount() << endl;
	}
	cout << "==================================" << endl;
	cout << "���� ��ȭ = " << shared_clientInfo->GetMyMoney() << endl;
	shared_clientInfo.get()->SendnMine("�α��� ����");
	ErrorHandStatic->ErrorHandler(SUCCES_LOGIN, shared_clientInfo);
	while (true)
	{
		string recvMessage;
		int isRecvSuccesResultValue = listener.Recvn(shared_clientInfo.get()->GetClientSocket(), recvMessage);
		if (SUCCES_RECV == isRecvSuccesResultValue)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			vector<string> commandMessage = ReadHandlerStatic->Parse(recvMessage, '/');
			commandController.CommandHandling(shared_clientInfo, commandMessage);
		}
		else if (ERROR_RECV == isRecvSuccesResultValue || ERROR_NULL_LINK_SEND == isRecvSuccesResultValue) // �޽��� �ޱ� ���� ���� ����
		{
			cout << "���� ������ ���Ͽ� �������� �������ϴ�." << endl;
			commandController.DeleteClientSocket(shared_clientInfo);
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
	
	
	CListener listener;
	string strNextUserNum = ReadHandlerStatic->GetNextUserNum(MakeNextJoinNumberTxt);
	int nextUserNum = stoi(strNextUserNum);
	CLobby lobby(nextUserNum);

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		readyNetWork.Accept(*clientSocket);
		
		thread clientThread(thSendRecv, clientSocket, &commandController, &listener, &lobby);
		clientThread.detach();
	}
	
	getchar();
}