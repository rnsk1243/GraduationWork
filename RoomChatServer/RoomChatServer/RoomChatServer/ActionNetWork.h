#pragma once
#include"Link.h"
#include"CommandController.h"
//#include"graduationWork.pb.h"

class CActionNetWork
{
	CActionNetWork(const CActionNetWork&);
	CActionNetWork& operator=(const CActionNetWork&);
	// Size ������
	int recvDataSource(SOCKET& recvOwnSocket, g_DataSize& g_dataSize, char* destination, int flags = 0); // �ø��������� ���� �ޱ�, �޾ƾ��ϴ� ũ��
	int recvSize(SOCKET& sock, g_DataSize& g_data, int flags = 0);
	int sendSize(SOCKET& sock, g_DataSize& g_data, int flags = 0);
	int multiSendn(g_DataSize&  g_data, LinkListIt iterBegin, LinkListIt iterEnd, CLink& sendOwnLink, LinkInfo* linkInfo, int flags = 0);
	// �� �̸� ������
	int sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
public:
	CActionNetWork();
	~CActionNetWork();
	int recvn(SOCKET& socket, g_Message& g_MS, g_DataSize& g_data, int flags = 0); // �ڽſ��Ը� ���� �ޱ�
	// ��� ���� �ޱ�
	int recvnData(CLink& clientInfo, g_DataSize& g_dataSize, int flags = 0);
	// ��� ���� ������
	int sendn(g_DataSize& g_data, CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	int sendn(g_DataSize& g_data, SOCKET & socket, int flags = 0); // �ڽſ��Ը� ������
};

