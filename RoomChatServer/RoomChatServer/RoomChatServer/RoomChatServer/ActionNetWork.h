#pragma once
#include"Link.h"
#include"CommandController.h"

class CActionNetWork
{
	MessageStruct mSendClientMessage; // ��� ó�� ����� Ŭ���̾�Ʈ���� ������ ���� �޼����� ���� ����
public:
	CActionNetWork();
	CActionNetWork(const CActionNetWork&) = delete;
	CActionNetWork& operator=(const CActionNetWork&) = delete;
	~CActionNetWork();
	// room���� �޽��� ������
	int Sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	// ���� ����ϴ� Ŭ���̾�Ʈ���Ը� ������
	int Sendn(SOCKET& socket, MessageStruct& MS, int flags = 0);
	int Recvn(shared_ptr<CLink> clientInfo, CCommandController& commandController, int flags = 0);
	// ���� ����ϴ� Ŭ���̾�Ʈ���� ���� �ޱ�
	int Recvn(SOCKET& socket, MessageStruct& MS, int flags = 0);
	int SendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
	// ���� ����ϴ� Ŭ���̾�Ʈ���� ����
	int AskClient(SOCKET& clientSocket, MessageStruct& MS, char* question);
	// ���� ����ϴ� Ŭ���̾�Ʈ���� �뺸�ϱ�
	int NotificationClient(SOCKET& clientSocket, MessageStruct& MS, char* notification);
};

