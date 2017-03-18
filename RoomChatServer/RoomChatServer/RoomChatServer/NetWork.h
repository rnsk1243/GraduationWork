#pragma once
#include"ChannelHandlere.h"
#include"RoomHandlere.h"
#include"Link.h"

const int IntSize = 4;
const int OccuredError = -1;
const int SuccesRecv = 1;
const int SuccesCommand = 2;
const int SuccesSend = 3;
const int EnterRoomPeopleLimit = 2;
const int MaxChannelNum = 5;

class CNetWork
{
	CLink* Link;
	CMyInfo* ClientInfo;
	CChannelHandlere* ChannelHandlere;
	CRoomHandlere* RoomHandlere;
	// ��� ó�� �Լ�(�� ����� ��)
	int commandHandling(char* command);
	bool deleteClientSocket(); // �ι� ���� ȣ�� �Ǹ� ���� private ����
public:
	CNetWork(CLink* link, CMyInfo* myInfo, CChannelHandlere* channelHandlere, CRoomHandlere* roomHandlere);
	~CNetWork();
	// room���� �޽��� ������
	int sendn(int flags = 0);
	int recvn(int flags = 0);
#pragma region get �Լ�
	//CChannelHandlere* getChannelHandlere() { return ChannelHandlere; }
	//CRoomHandlere* getRoomHandlere() { return RoomHandlere; }
#pragma endregion
};