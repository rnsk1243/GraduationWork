#pragma once
#include"Link.h"
#include"CommandController.h"
//#include"graduationWork.pb.h"

class CActionNetWork
{
	CActionNetWork(const CActionNetWork&);
	CActionNetWork& operator=(const CActionNetWork&);
	// Size ������
	int sendSize(SOCKET& sock, int size, g_DataType type, int flags = 0);
	int recvSize(SOCKET& sock, g_DataSize& g_size, int flags = 0);
public:
	CActionNetWork();
	~CActionNetWork();
	// room���� �޽��� ������
	int sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	int sendn(SOCKET& socket, g_Message& g_MS, int flags = 0); // �ڽſ��Ը� ������
	int recvn(CLink& clientInfo, CCommandController& commandController, int flags = 0);
	int recvn(SOCKET& socket, g_Message& g_MS, int flags = 0); // �ڽſ��Ը� ���� �ޱ�
	int sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
	// transform �ޱ�
	int recvnTransform(SOCKET& clientSocket, CLink& clientInfo, int recvSize, int flags = 0);
	// message �ޱ�
	int recvnMessage(SOCKET& clientSocket, CLink& clientInfo, int recvSize, int flags = 0);
};

