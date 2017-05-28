#pragma once
#include"Link.h"
#include"CommandController.h"



class CActionNetWork
{
	MessageStruct sendClientMessage; // ��� ó�� ����� Ŭ���̾�Ʈ���� ������ ���� �޼����� ���� ����
public:
	CActionNetWork();
	CActionNetWork(const CActionNetWork&) = delete;
	CActionNetWork& operator=(const CActionNetWork&) = delete;
	~CActionNetWork();
	// room���� �޽��� ������
	int sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	// ���� ����ϴ� Ŭ���̾�Ʈ���Ը� ������
	int sendn(SOCKET& socket, MessageStruct& MS, int flags = 0);
	int recvn(shared_ptr<CLink> clientInfo, CCommandController& commandController, int flags = 0);
	// ���� ����ϴ� Ŭ���̾�Ʈ���� ���� �ޱ�
	int recvn(SOCKET& socket, MessageStruct& MS, int flags = 0);
	int sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
	// ���� ����ϴ� Ŭ���̾�Ʈ���� ����
	int askClient(SOCKET& clientSocket, MessageStruct& MS, char* question);
	// ���� ����ϴ� Ŭ���̾�Ʈ���� �뺸�ϱ�
	int notificationClient(SOCKET& clientSocket, MessageStruct& MS, char* notification);
};

