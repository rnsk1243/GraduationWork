#pragma once
#include"Link.h"
#include"CommandController.h"

//typedef vector<SOCKET> SocketVec;
//typedef SocketVec::iterator SocketVecIt;

class CActionNetWork
{
	//MessageStruct mSendClientMessage; // ��� ó�� ����� Ŭ���̾�Ʈ���� ������ ���� �޼����� ���� ����
public:
	CActionNetWork();
	CActionNetWork(const CActionNetWork&) = delete;
	CActionNetWork& operator=(const CActionNetWork&) = delete;
	~CActionNetWork();
	// room���� �޽��� ������
	int Sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	int Recvn(const shared_ptr<CLink>& clientInfo, CCommandController& commandController, int flags = 0);
	int SendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
	int MultiSendn(SocketVec& sockets, const string& MS, int flags = 0);
	// Ŭ���̾�Ʈ �Ѹ��� ������
	int Sendn(SOCKET& socket, const string& strMessage, int flags = 0);
	// ���� ����ϴ� Ŭ���̾�Ʈ���� ���� �ޱ�
	int Recvn(SOCKET& socket, string& strMessage, int flags = 0);
};

