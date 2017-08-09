#pragma once
#include<iostream>
using namespace std;
#include"Link.h"
#include"RoomManager.h"


class CRoomHandler
{
	bool GetMyRoomIter(CLink* clientInfo, CRoomManager* roomManager, RoomListIt& resultRoomIter);
public:
	CRoomHandler(const CRoomHandler&) = delete;
	CRoomHandler& operator=(const CRoomHandler&) = delete;
	CRoomHandler();
	~CRoomHandler();
	// room에서 나가기 (나가고자하는 소켓)
	bool ExitRoom(CLink* clientInfo, CRoomManager* roomManager);
	// 현재 자신이 속한 방이 없을때 방 만들기(들어갈 소켓, 만드는 방 번호) 
	bool MakeRoom(const shared_ptr<CLink>& shared_clientInfo, CRoomManager* roomManager, const string& roomName,const int& battingMoney);
	// 현재 자신이 속한 방이 없을때 방 입장 하기
	bool EnterRoom(const shared_ptr<CLink>& shared_clientInfo, CRoomManager* roomManager, int targetRoomNo);
	// 방 제목 추출 함수
	//char* returnRoomName(char* message);
	// 방에 있는 사람 전부 배팅 준비 됐나?
	bool IsAllReadyGame(CLink* clientInfo, CRoomManager* roomManager);
	// 배팅 결과 보여줄 준비 됐나?
	bool IsAllReadyBatting(CLink* clientInfo, CRoomManager* roomManager);
};

