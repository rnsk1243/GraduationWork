#include "Room.h"
#include"ErrorHandler.h"


CRoom::CRoom(int roomNum, int channelNum, const string& roomName, const int& battingMoney) :
	mRoomNum(roomNum),
	mChannelNum(channelNum),
	mRoomName(roomName),
	mAmountPeople(0),
	mBettingMoney(battingMoney),
	mPlayingGame(false)
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
	// 인원수가 모자르면 무조건 false;
	if (EnterRoomPeopleLimit > mAmountPeople)
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
	SetGame();
	return true;
}

bool CRoom::BattingResult(int& resultPK, bool& isDrawResult)
{
	// 인원수가 모자르면 무조건 false;
	if (EnterRoomPeopleLimit > mAmountPeople)
	{
		ErrorHandStatic->ErrorHandler(ERROR_BATTING_RESULT_ALONE);
		return false;
	}
	LinkListIt iterBegin = GetIterMyInfoBegin();
	CLink* winner = (*iterBegin).get();
	int curWinnerStat = CardStatic->GetCardStat(winner->GetMyBattingCardNumber()); // 현재 가장 높은 스텟 크기
	bool isDraw = true; // 최고 높은 스텟을 낸 사람이 둘 이상인가? 
	++iterBegin;
	for (; iterBegin != GetIterMyInfoEnd(); ++iterBegin)
	{
		if ((*iterBegin).use_count() > 0)
		{
			CLink* client = (*iterBegin).get();
			const int challengerStat = CardStatic->GetCardStat(client->GetMyBattingCardNumber());
			if (curWinnerStat < challengerStat)
			{
				curWinnerStat = challengerStat;
				winner = client;
				isDraw = false;
			}
			else if (curWinnerStat == challengerStat)
			{
				// 비김
				isDraw = true;
			}
		}
		else
		{
			ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
			return false;
		}
	}
	isDrawResult = isDraw;
	if (false == isDraw)
	{
		resultPK = winner->GetMyPKNumber();
		winner->GetPrizeBattingMoney(mBettingMoney);
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

bool CRoom::GetRoomSockets(vector<SOCKET>& roomSockets, bool isMyInclude, const SOCKET* myClientSock)
{
	LinkListIt linkBegin = GetIterMyInfoBegin();
	for (; linkBegin != GetIterMyInfoEnd(); ++linkBegin)
	{
		if ((*linkBegin).use_count() > 0)
		{
			if (true == isMyInclude)
			{
				roomSockets.push_back((*linkBegin).get()->GetClientSocket());
			}
			else
			{
				if (nullptr == myClientSock)
				{
					ErrorHandStatic->ErrorHandler(ERROR_GETROOMSOCKET_MYCLIENT_NULLPTR);
					return false;
				}
				if (*myClientSock != (*linkBegin).get()->GetClientSocket())
				{
					roomSockets.push_back((*linkBegin).get()->GetClientSocket());
				}
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

bool CRoom::AllCalculateMoney()
{
	LinkListIt linkBegin = mClientInfos.begin();
	bool isSaveResult = true; // 모두 .txt에 저장 되었나? 한 명이라도 안되면 false 반환
	for (; linkBegin != GetIterMyInfoEnd(); ++linkBegin)
	{
		if ((*linkBegin).use_count() > 0)
		{
			if (false == (*linkBegin).get()->SaveCalculateMoney())
			{
				ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY, (*linkBegin).get());
				isSaveResult = false;
				continue;
			}
		}
		else
		{
			ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
			isSaveResult = false;
		}
	}
	return isSaveResult;
}

bool CRoom::AllInitBetting()
{
	LinkListIt linkBegin = mClientInfos.begin();
	for (; linkBegin != GetIterMyInfoEnd(); ++linkBegin)
	{
		if ((*linkBegin).use_count() > 0)
		{
			(*linkBegin).get()->InitBetting();
		}
	}
	SetGameOver();
	return true;
}

bool CRoom::AllRefundBettingMoney()
{
	LinkListIt linkBegin = mClientInfos.begin();
	for (; linkBegin != GetIterMyInfoEnd(); ++linkBegin)
	{
		if ((*linkBegin).use_count() > 0)
		{
			(*linkBegin).get()->RefundBettingMoney(mBettingMoney);
		}
	}
	return true;
}


// 모두 카드 냈나?
bool CRoom::IsAllReadyBetting()
{
	// 인원수가 모자르면 무조건 false;
	if (EnterRoomPeopleLimit > mAmountPeople)
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
	// 모두 카드 냄
	return true;
}

