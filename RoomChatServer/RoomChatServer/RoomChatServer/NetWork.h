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
	// 명령 처리 함수(방 만들기 등)
	int commandHandling(char* command);
	bool deleteClientSocket(); // 두번 연속 호출 되면 오류 private 숨김
public:
	CNetWork(CLink* link, CMyInfo* myInfo, CChannelHandlere* channelHandlere, CRoomHandlere* roomHandlere);
	~CNetWork();
	// room에서 메시지 보내기
	int sendn(int flags = 0);
	int recvn(int flags = 0);
#pragma region get 함수
	//CChannelHandlere* getChannelHandlere() { return ChannelHandlere; }
	//CRoomHandlere* getRoomHandlere() { return RoomHandlere; }
#pragma endregion
};