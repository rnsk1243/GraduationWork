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
	// ���� ����ִ� �� �ο�
	int mAmountPeople;
	MUTEX mRAII_RoomMUTEX;
	//CRITICALSECTION CT;
	int mBettingMoney;
	bool mPlayingGame; // ������?
	//int mReadyPeopleAmount; // �غ�� ��� ��
	void IncreasePeople() { mAmountPeople++; }
	void DecreasePeople() { if (mAmountPeople > 0) mAmountPeople--; }
	void SetGame() { mPlayingGame = true; }
	void SetGameOver() { mPlayingGame = false; }
public:
	CRoom(const CRoom&) = delete;
	CRoom& operator=(const CRoom&) = delete;
	CRoom(int roomNum,int channelNum, const string& roomName,const int& battingMoney);
	~CRoom();
#pragma region push, erase �Լ�
	void PushClient(shared_ptr<CLink> shared_client);
	LinkListIt EraseClient(LinkListIt myInfoListIt);
#pragma endregion
#pragma region get,set �Լ�
	int GetRoomNum() { return mRoomNum; }
	int GetChannelNum() { return mChannelNum; }
	const string GetRoomName() { return mRoomName; }
	LinkListIt GetIterMyInfoBegin() { return mClientInfos.begin(); }
	LinkListIt GetIterMyInfoEnd() { return mClientInfos.end(); }
	int GetAmountPeople() { return mAmountPeople; }
	int GetBattingMoney() { return mBettingMoney; }
	//void IncreaseReadyPeople() { ++mReadyPeopleAmount; }
#pragma endregion
	bool MergeRoom(CRoom* targetRoom);
	bool IsAllReadyBetting();
	bool IsAllReady();
	bool BattingResult(int& resultPK, bool& isDrawResult);
	// �濡 ���� �ִ� ���� ��������.(��� ���� ����)
	bool GetRoomSockets(vector<SOCKET>& roomSockets, bool isMyInclude, const SOCKET* myClientSock = nullptr);
	bool AllCalculateMoney(); // �濡 �ִ� ��� ��� ����(.txt ����)
	bool AllInitBetting();
	bool AllRefundBettingMoney(); // ��� ������� ���� �ݾ� ȯ��
	bool IsGame() { return mPlayingGame; }
};

