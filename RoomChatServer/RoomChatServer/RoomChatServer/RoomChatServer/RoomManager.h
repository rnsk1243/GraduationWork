#pragma once
#include<list>
#include<iostream>
#include"Room.h"

class CLink;
class CRoom;
using namespace std;
#pragma region 타입재정의

typedef shared_ptr<CRoom> RoomPtr;
typedef list<RoomPtr> RoomList;
typedef RoomList::const_iterator RoomListIt;
#pragma endregion

class CRoomManager
{
	RoomList mRooms;
//	CRITICAL_SECTION CS_Room;
	MUTEX mRAII_RoomManagerMUTEX;
	RoomListIt GetMyRoomIter(int ChannelNum, int roomNum);
	int newRoomNumber;
	void PushRoom(const RoomPtr& shared_newRoom);
public:
	CRoomManager();
	CRoomManager(const CRoomManager&) = delete;
	CRoomManager& operator=(const CRoomManager&) = delete;
	~CRoomManager();

	RoomListIt EraseRoom(RoomListIt deleteTargetRoomIter);

	////////////
	// room에서 나가기 (나가고자하는 소켓)
	RoomListIt ExitRoom(const LinkPtr & shared_clientInfo);
	// 현재 자신이 속한 방이 없을때 방 만들기(들어갈 소켓, 만드는 방 번호) 
	int MakeRoom(const LinkPtr & shared_clientInfo, const string& roomName, const int& battingMoney);
	// 현재 자신이 속한 방이 없을때 방 입장 하기
	bool EnterRoom(const LinkPtr& shared_clientInfo, int targetRoomNumBer);
	// 방에 있는 사람 전부 배팅 준비 됐나?
	bool IsAllReadyGame(const LinkPtr& shared_clientInfo);
	// 배팅 결과 보여줄 준비 됐나?
	bool IsAllReadyBatting(const LinkPtr& shared_clientInfo);
	// 승리 판별
	void ResultBatting(const LinkPtr& shared_clientInfo);
	// 해당 링크가 들어있는 방 방송
	void Broadcast(const LinkPtr& shared_clientInfo, const string& message, int flags = 0);
	// 해당 링크가 들어있는 수다
	void Talk(const LinkPtr& shared_clientInfo, const string& message, int flags = 0);
};
 
