#include "RoomManager.h"
#include"ErrorHandler.h"
#include "RoomHandler.h"

CRoomManager::CRoomManager():newRoomNumber(StartRoomNum)
{
}


CRoomManager::~CRoomManager()
{
}

RoomListIt CRoomManager::GetMyRoomIter(int ChannelNum, int roomNum)
{
	RoomListIt iterBegin = mRooms.begin();
	RoomListIt iterEnd = mRooms.end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		// 채널이 같은지 확인
		if (ChannelNum == (*iterBegin)->GetChannelNum())
		{
			// 채널이 같고 룸번호 까지 같은지 확인
			if (roomNum == (*iterBegin)->GetRoomNum())
			{
				return iterBegin;
			}
		}
	}
	cout << roomNum << "번 방이 없습니다." << endl;
	ErrorHandStatic->ErrorHandler(ERROR_GET_ROOM);
	//RoomListIt* error = nullptr;
	return iterBegin; // iterBegin == iterEnd 이면 방이없다.
}


void CRoomManager::PushRoom(const RoomPtr & shared_newRoom)
{
	ScopeLock<MUTEX> MU(mRAII_RoomManagerMUTEX);
	mRooms.push_back(shared_newRoom);
	++newRoomNumber;
}

RoomListIt CRoomManager::EraseRoom(RoomListIt deleteTargetRoomIter)
{
	ScopeLock<MUTEX> MU(mRAII_RoomManagerMUTEX);
	RoomListIt delRoom = mRooms.erase(deleteTargetRoomIter);
	return delRoom;
}


RoomListIt CRoomManager::ExitRoom(const LinkPtr & shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return mRooms.end();
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		string outClientName(shared_clientInfo.get()->GetMyName() + " 님이 방에서 나가셨습니다.");
		(*myRoomIter).get()->Talk(shared_clientInfo, outClientName);
		client->SetMyRoomNum(NoneRoom);
		client->InitBetting(); // 베팅 초기화 시킴
		if (true == (*myRoomIter)->IsGame())					// 게임중에 나갔나?
		{
			client->LostWillMoney((*myRoomIter)->GetBattingMoney());	// 벌금 부과
			client->SaveCalculateMoney(); // 갈땐 가더라도 정산은..해야지
			(*myRoomIter)->AllInitBetting();					// 룸에 들어있는 사람 준비 초기화
		}
		(*myRoomIter).get()->EraseClient(shared_clientInfo);
		return myRoomIter;
	}
	return mRooms.end();
}

int CRoomManager::MakeRoom(const LinkPtr & shared_clientInfo, const string& roomName, const int& battingMoney)
{
	CLink* client = shared_clientInfo.get();
	if (client->GetMyMoney() >= battingMoney && battingMoney >= 0)
	{
		RoomPtr newRoom(new CRoom(newRoomNumber, client->GetMyChannelNum(), roomName, battingMoney));
		int makedRoomNumber = newRoomNumber; // PushRoom을 호출하면 newRoomNumber가 1 증가하기때문에
		PushRoom(newRoom);
		return makedRoomNumber;
	}
	else
	{
		client->SendnMine(MakeRoomMoneyLack);
		return ErrorHandStatic->ErrorHandler(ERROR_ROOM_ENTRER_BATTING_MONEY);
	}
}

bool CRoomManager::EnterRoom(const LinkPtr & shared_clientInfo, int targetRoomNumBer)
{
	CLink* client = shared_clientInfo.get(); 
	if (nullptr == client)
		return false;
	if (true == client->IsRoomEnterState()) // 이미 방에 있는지 확인
		return false;
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), targetRoomNumBer);
	if (EnterRoomPeopleLimit <= (*targetRoomIter).get()->GetAmountPeople())
	{
		shared_clientInfo.get()->SendnMine(DialogEnterRoomPeopleLimit);
		return false;
	}
	if (mRooms.end() != targetRoomIter)
	{
		int BattingMoney = (*targetRoomIter)->GetBattingMoney();
		if (BattingMoney <= client->GetMyMoney())
		{
			(*targetRoomIter)->PushClient(shared_clientInfo, targetRoomNumBer);
			return true;
		}
		else
		{
			shared_clientInfo.get()->SendnMine(EnterRoomMoneyLack);
			ErrorHandStatic->ErrorHandler(ERROR_ROOM_ENTRER_BATTING_MONEY, shared_clientInfo);
		}
	}
	return false;
}

bool CRoomManager::IsAllReadyGame(const LinkPtr & shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return false;
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != targetRoomIter)
	{
		client->SetReadyGame((*targetRoomIter).get()->GetBattingMoney());
		return ((*targetRoomIter)->IsAllReady());
	}
	return false;
}


void CRoomManager::ResultBatting(const LinkPtr& shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return;
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (true == (*myRoomIter).get()->IsAllReadyBetting())
	{
		(*myRoomIter).get()->BattingResult();
	}
}

void CRoomManager::Broadcast(const LinkPtr & shared_clientInfo, const string & message, int flags)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return;
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		(*myRoomIter).get()->Broadcast(message, flags);
	}
}

void CRoomManager::Talk(const LinkPtr & shared_clientInfo, const string & message, int flags)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return;
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		(*myRoomIter).get()->Talk(shared_clientInfo, message, flags);
	}
}



