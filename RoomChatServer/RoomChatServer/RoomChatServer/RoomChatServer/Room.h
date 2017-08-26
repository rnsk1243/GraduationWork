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
	const int mChannelNum;
	const int mRoomNum;
	// 현재 들어있는 방 인원
	int mAmountPeople;
	MUTEX mRAII_RoomMUTEX;
	//CRITICALSECTION CT;
	int mBettingMoney;
	bool mPlayingGame; // 게임중?
	void IncreasePeople();
	void DecreasePeople();
	void SetGame();
	void SetGameOver();
	void SendBattingResult(const LinkPtr& winner, int flags = 0);
	bool AllCalculateMoney(); // 방에 있는 사람 모두 정산(.txt 저장)
public:
	CRoom(const CRoom&) = delete;
	CRoom& operator=(const CRoom&) = delete;
	CRoom(int roomNum,int channelNum, const string& roomName,const int& battingMoney);
	~CRoom();
	void PushClient(const LinkPtr& shared_client, const int& enterRoomNumber);
	LinkListIt EraseClient(const LinkPtr& shared_client);
	int GetRoomNum();
	int GetChannelNum();
	const string GetRoomName();
	int GetAmountPeople();
	bool IsRoomEmpty();
	int GetBattingMoney();
	bool IsAllReadyBetting();
	bool IsAllReady();
	void BattingResult();
	bool IsGame();
	bool AllInitBetting();
	void Broadcast(const string& message, int flags = 0);
	void Talk(const LinkPtr& myClient, const string& message, int flags = 0);

};

