#include "Room.h"
#include"ErrorHandler.h"


CRoom::CRoom(int roomNum, int channelNum, const string& roomName,const int& battingMoney) :
	mRoomNum(roomNum),
	mChannelNum(channelNum),
	mRoomName(roomName),
	mAmountPeople(0),
	mBattingMoney(battingMoney)
{
	//InitializeCriticalSection(&CS_MyInfoList);
}


CRoom::~CRoom()
{
	cout << mRoomNum << " 번 방이 삭제 됩니다." << endl;
	//if (ClientInfos.empty())
	//{
	//	LinkListIt begin = getIterMyInfoBegin();
	//	LinkListIt end = getIterMyInfoEnd();
	//	for (; begin != end; ++begin)
	//	{
	//		delete(*begin);
	//	}
	//	ClientInfos.clear();
	//	//DeleteCriticalSection(&CS_MyInfoList);
	//}else
	//{
	//	cout << "방 삭제 실패" << endl;
	//}
	cout << "방 삭제 완료" << endl;
}

void CRoom::PushClient(shared_ptr<CLink> shared_client)
{
	ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
	mClientInfos.push_back(shared_client);
	IncreasePeople();
}

LinkListIt CRoom::EraseClient(LinkListIt myInfoListIt)
{
	LinkListIt temp;
	{
		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
		temp = mClientInfos.erase(myInfoListIt);
		DecreasePeople();
	}
	return temp;
}

bool CRoom::MergeRoom(CRoom * targetRoom)
{
	{
		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX); // rock0
		{
			ScopeLock<MUTEX> MU(targetRoom->mRAII_RoomMUTEX); // rock1
															  // 실제 옮기기 전에 준비작업으로 room정보 수정
#pragma region 옮기는 방안에 있는 클라이언트들의 room정보 수정(방 번호라든지..)
			LinkListIt linkBegin = targetRoom->GetIterMyInfoBegin();
			LinkListIt linkEnd = targetRoom->GetIterMyInfoEnd();
			for (; linkBegin != linkEnd; ++linkBegin)
			{
				CLink* targetClient = (*linkBegin).get();
				targetClient->SetMyRoomNum(mRoomNum);
				IncreasePeople(); // 방 인원수 갱신
			}
#pragma endregion 
			//				ClientInfos.sort();
			//				targetRoom->ClientInfos.sort();
			mClientInfos.merge(targetRoom->mClientInfos); // 실제 옮김
		} // rock1 unlock
	} // rock0 unlock
	return true;
}

bool CRoom::IsAllReady()
{
	// 나 혼자 이면 무조건 false;
	if (1 == mAmountPeople)
	{
		return false;
	}
	LinkListIt iterBegin = GetIterMyInfoBegin();
	for (; iterBegin != GetIterMyInfoEnd(); ++iterBegin)
	{
		if ((*iterBegin).use_count() > 0)
		{
			CLink* client = (*iterBegin).get();
			if (false == client->GetReadyGame())
			{
				return false;
			}
		}
		else
		{
			ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
			return false;
		}
	}
	return true;
}

CLink * CRoom::BattingResult()
{
	// 나 혼자 이면 무조건 false;
	if (1 == mAmountPeople)
	{
		return nullptr;
	}
	LinkListIt iterBegin = GetIterMyInfoBegin();
	CLink* winner = (*iterBegin).get();
	for (; iterBegin != GetIterMyInfoEnd(); ++iterBegin)
	{
		if ((*iterBegin).use_count() > 0)
		{
			CLink* client = (*iterBegin).get();
			if (CardStatic->GetCardStat(winner->GetMyBattingCardNumber()) < CardStatic->GetCardStat(client->GetMyBattingCardNumber()))
			{
				winner = client;
			}
		}
		else
		{
			ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
			return nullptr;
		}
	}
	return winner;
}

bool CRoom::IsAllReadyBatting()
{
	// 나 혼자 이면 무조건 false;
	if (1 == mAmountPeople)
	{
		return false;
	}
	LinkListIt iterBegin = GetIterMyInfoBegin();
	for (; iterBegin != GetIterMyInfoEnd(); ++iterBegin)
	{
		if ((*iterBegin).use_count() > 0)
		{
			CLink* client = (*iterBegin).get();
			if (false == client->GetReadyBatting())
			{
				return false;
			}
		}
		else
		{
			ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
			return false;
		}
	}
	return true;
}

