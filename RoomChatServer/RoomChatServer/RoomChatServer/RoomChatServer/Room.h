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
	void IncreasePeople() { mAmountPeople++; }
	void DecreasePeople() { if (mAmountPeople > 0) mAmountPeople--; }
public:
	CRoom(const CRoom&) = delete;
	CRoom& operator=(const CRoom&) = delete;
	CRoom(int roomNum,int channelNum, const string& roomName);
	~CRoom();
#pragma region push, erase �Լ�
	void PushClient(shared_ptr<CLink> shared_client)
	{
		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
		mClientInfos.push_back(shared_client);
		IncreasePeople();
	}
	LinkListIt EraseClient(LinkListIt myInfoListIt)
	{
		LinkListIt temp;
		{
			ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
			temp = mClientInfos.erase(myInfoListIt);
			DecreasePeople();
		}
		return temp;
	}
#pragma endregion
#pragma region get,set �Լ�
	int GetRoomNum() { return mRoomNum; }
	int GetChannelNum() { return mChannelNum; }
	const string GetRoomName() { return mRoomName; }
	LinkListIt GetIterMyInfoBegin() { return mClientInfos.begin(); }
	LinkListIt GetIterMyInfoEnd() { return mClientInfos.end(); }
	int GetAmountPeople() { return mAmountPeople; }
#pragma endregion
	bool MergeRoom(CRoom* targetRoom)
	{
		{
			ScopeLock<MUTEX> MU(mRAII_RoomMUTEX); // rock0
			{
				ScopeLock<MUTEX> MU(targetRoom->mRAII_RoomMUTEX); // rock1
				// ���� �ű�� ���� �غ��۾����� room���� ����
#pragma region �ű�� ��ȿ� �ִ� Ŭ���̾�Ʈ���� room���� ����(�� ��ȣ�����..)
				LinkListIt linkBegin = targetRoom->GetIterMyInfoBegin();
				LinkListIt linkEnd = targetRoom->GetIterMyInfoEnd();
				for (; linkBegin != linkEnd; ++linkBegin)
				{
					CLink* targetClient = (*linkBegin).get();
					targetClient->SetMyRoomNum(mRoomNum);
					IncreasePeople(); // �� �ο��� ����
				}
#pragma endregion 
//				ClientInfos.sort();
//				targetRoom->ClientInfos.sort();
				mClientInfos.merge(targetRoom->mClientInfos); // ���� �ű�
			} // rock1 unlock
		} // rock0 unlock
		return true;
	}
};

