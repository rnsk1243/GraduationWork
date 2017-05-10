#pragma once
#include"Link.h"
#include"CommandController.h"
//#include"graduationWork.pb.h"

class CActionNetWork
{
	CActionNetWork(const CActionNetWork&);
	CActionNetWork& operator=(const CActionNetWork&);
	// Size ������
	int sendSize(SOCKET& sock, int size, DataType type, int flags = 0);
	int recvSize(SOCKET& sock, DataSize& g_size, int flags = 0);
public:
	CActionNetWork();
	~CActionNetWork();
	// room���� �޽��� ������
	int sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	// �ڽſ��� ������
	int sendn(SOCKET& socket, Message& g_MS, int flags = 0);
	int recvn(CLink& clientInfo, CCommandController& commandController, int flags = 0);
	// �ڽſ��� ���� �ޱ�
	int recvn(SOCKET& socket, Message& g_MS, int flags = 0);
	int sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
};

