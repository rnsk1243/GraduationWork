#pragma once
#include<iostream>
using namespace std;
#include"Link.h"
#include"RoomManager.h"


class CRoomHandler
{
	CRoomHandler(const CRoomHandler&);
	CRoomHandler& operator=(const CRoomHandler&);
public:
	CRoomHandler();
	~CRoomHandler();
	// room에서 나가기 (나가고자하는 소켓)
	bool exitRoom(CLink* clientInfo, CRoomManager* roomManager);
	// 현재 자신이 속한 방이 없을때 방 만들기(들어갈 소켓, 만드는 방 번호) 
	bool makeRoom(CLink* clientInfo, CRoomManager* roomManager, char* roomName);
	// 현재 자신이 속한 방이 없을때 방 입장 하기
	bool enterRoom(CLink* clientInfo, CRoomManager* roomManager, int targetRoomNo);
	// 방 제목 추출 함수
	char* returnRoomName(char* message);
};

