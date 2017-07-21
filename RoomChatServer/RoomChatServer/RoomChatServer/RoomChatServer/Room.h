#pragma once
#include<WinSock2.h>
#include<iostream>
#include<list>
#include"Channel.h"
#include"RAII.h"
class CChannel;
class CLink;
using namespace std;

class CRoom
{
	LinkList mClientInfos;
	string mRoomName;
	int mChannelNum;
	int mRoomNum;
	// 현재 들어있는 방 인원
	int mAmountPeople;
	MUTEX mRAII_RoomMUTEX;
	//CRITICALSECTION CT;
	int mBattingMoney;
	
	//int mReadyPeopleAmount; // 준비된 사람 수
	void IncreasePeople() { mAmountPeople++; }
	void DecreasePeople() { if (mAmountPeople > 0) mAmountPeople--; }
public:
	CRoom(const CRoom&) = delete;
	CRoom& operator=(const CRoom&) = delete;
	CRoom(int roomNum,int channelNum, const string& roomName,const int& battingMoney);
	~CRoom();
#pragma region push, erase 함수
	void PushClient(shared_ptr<CLink> shared_client);
	LinkListIt EraseClient(LinkListIt myInfoListIt);
#pragma endregion
#pragma region get,set 함수
	int GetRoomNum() { return mRoomNum; }
	int GetChannelNum() { return mChannelNum; }
	const string GetRoomName() { return mRoomName; }
	LinkListIt GetIterMyInfoBegin() { return mClientInfos.begin(); }
	LinkListIt GetIterMyInfoEnd() { return mClientInfos.end(); }
	int GetAmountPeople() { return mAmountPeople; }
	int GetBattingMoney() { return mBattingMoney; }
	//void IncreaseReadyPeople() { ++mReadyPeopleAmount; }
#pragma endregion
	bool MergeRoom(CRoom* targetRoom);
	bool IsAllReadyBatting();
	bool IsAllReady();
	int BattingResult();
	// 방에 속해 있는 소켓 가져오기.(결과 담을 벡터)
	bool GetRoomSockets(vector<SOCKET>& roomSockets);
};

