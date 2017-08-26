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
}

void CRoom::PushClient(const LinkPtr& shared_client, const int& enterRoomNumber)
{
	ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
	mClientInfos.push_back(shared_client);
	shared_client.get()->SetMyRoomNum(enterRoomNumber);
	Broadcast(shared_client.get()->GetMyName() + "님이 방에 입장 하셨습니다.");
	IncreasePeople();
}

LinkListIt CRoom::EraseClient(const LinkPtr& shared_client)
{
	LinkListIt delLinkIter = find(mClientInfos.begin(), mClientInfos.end(), shared_client);
	{
		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
		delLinkIter = mClientInfos.erase(delLinkIter);
		DecreasePeople();
	}
	return delLinkIter;
}

int CRoom::GetRoomNum()
{
	return mRoomNum;
}

int CRoom::GetChannelNum()
{
	return mChannelNum;
}

const string CRoom::GetRoomName()
{
	return mRoomName;
}

int CRoom::GetAmountPeople()
{
	return mAmountPeople;
}

bool CRoom::IsRoomEmpty()
{
	if (0 >= mAmountPeople)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

int CRoom::GetBattingMoney()
{
	return mBettingMoney;
}

//bool CRoom::MergeRoom(CRoom * targetRoom)
//{
//	{
//		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX); // rock0
//		{
//			ScopeLock<MUTEX> MU(targetRoom->mRAII_RoomMUTEX); // rock1
//															  // 실제 옮기기 전에 준비작업으로 room정보 수정
//			LinkListIt linkBegin = targetRoom->GetIterMyInfoBegin();
//			LinkListIt linkEnd = targetRoom->GetIterMyInfoEnd();
//			for (; linkBegin != linkEnd; ++linkBegin)
//			{
//				CLink* targetClient = (*linkBegin).get();
//				targetClient->SetMyRoomNum(mRoomNum);
//				IncreasePeople(); // 방 인원수 갱신
//			}
//			mClientInfos.merge(targetRoom->mClientInfos); // 실제 옮김
//		} // rock1 unlock
//	} // rock0 unlock
//	return true;
//}

bool CRoom::IsAllReady()
{
	// 인원수가 모자르면 무조건 false;
	if (EnterRoomPeopleLimit > mAmountPeople)
	{
		return false;
	}
	LinkListIt iterBegin = mClientInfos.begin();
	for (; iterBegin != mClientInfos.end(); ++iterBegin)
	{
		CLink* client = (*iterBegin).get();
		if (false == client->GetReadyGame())
		{
			return false;
		}
	}
	SetGame();
	return true;
}

void CRoom::BattingResult()
{
	LinkListIt iterBegin = mClientInfos.begin();
	LinkPtr winner = (*iterBegin);
	winner.get()->LostWillMoney(mBettingMoney);
	int curWinnerStat = CardStatic->GetCardStat(winner->GetMyBattingCardNumber()); // 현재 가장 높은 스텟 크기
	bool isDraw = false; // 최고 높은 스텟을 낸 사람이 둘 이상인가? 
	++iterBegin;
	for (; iterBegin != mClientInfos.end(); ++iterBegin)
	{
		LinkPtr client = (*iterBegin);
		client.get()->LostWillMoney(mBettingMoney);
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

	if (false == isDraw)
	{
		winner->GetPrizeBattingMoney(mBettingMoney);
		AllCalculateMoney();
		SendBattingResult(winner);
	}
	else
	{
		Broadcast("승자가 없습니다. 무효게임");
	}
	AllInitBetting();
	
}


bool CRoom::AllCalculateMoney()
{
	LinkListIt linkBegin = mClientInfos.begin();
	bool isSaveResult = true; // 모두 .txt에 저장 되었나? 한 명이라도 안되면 false 반환
	for (; linkBegin != mClientInfos.end(); ++linkBegin)
	{
		if (false == (*linkBegin).get()->SaveCalculateMoney())
		{
			ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY, (*linkBegin));
			isSaveResult = false;
			continue;
		}
	}
	return isSaveResult;
}

bool CRoom::AllInitBetting()
{
	LinkListIt linkBegin = mClientInfos.begin();
	for (; linkBegin != mClientInfos.end(); ++linkBegin)
	{
		(*linkBegin).get()->InitBetting();
	}
	SetGameOver();
	return true;
}

bool CRoom::IsGame()
{
	return mPlayingGame;
}

void CRoom::Broadcast(const string& message, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		(*clientIterBegin).get()->SendnMine(message, flags);
	}
}

void CRoom::Talk(const LinkPtr& myClient, const string & message, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	LinkListIt myIter = find(mClientInfos.begin(), mClientInfos.end(), myClient);
	if (mClientInfos.end() == myIter)
	{
		Broadcast(message, flags);
		return;
	}
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		if (clientIterBegin != myIter)
		{
			(*clientIterBegin).get()->SendnMine(message, flags);
		}
	}
}

void CRoom::IncreasePeople()
{
	mAmountPeople++;
}

void CRoom::DecreasePeople()
{
	if (mAmountPeople > 0) mAmountPeople--;
}

void CRoom::SetGame()
{
	mPlayingGame = true;
}

void CRoom::SetGameOver()
{
	mPlayingGame = false;
}

void CRoom::SendBattingResult(const LinkPtr& winner, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	LinkListIt winnerIter = find(mClientInfos.begin(), mClientInfos.end(), winner);
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		if (clientIterBegin != winnerIter)
		{
			(*clientIterBegin).get()->SendnMine("당신은 졌습니다.", flags);
		}
		else
		{
			(*clientIterBegin).get()->SendnMine("당신은 이겼습니다.", flags);
		}
	}
}


// 모두 카드 냈나?
bool CRoom::IsAllReadyBetting()
{
	// 인원수가 모자르면 무조건 false;
	if (EnterRoomPeopleLimit > mAmountPeople)
	{
		return false;
	}
	LinkListIt iterBegin = mClientInfos.begin();
	for (; iterBegin != mClientInfos.end(); ++iterBegin)
	{
		CLink* client = (*iterBegin).get();
		if (false == client->GetReadyBatting())
		{
			return false;
		}
	}
	// 모두 카드 냄
	return true;
}

