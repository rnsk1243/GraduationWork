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
		// ä���� ������ Ȯ��
		if (ChannelNum == (*iterBegin)->GetChannelNum())
		{
			// ä���� ���� ���ȣ ���� ������ Ȯ��
			if (roomNum == (*iterBegin)->GetRoomNum())
			{
				return iterBegin;
			}
		}
	}
	cout << roomNum << "�� ���� �����ϴ�." << endl;
	ErrorHandStatic->ErrorHandler(ERROR_GET_ROOM);
	//RoomListIt* error = nullptr;
	return iterBegin; // iterBegin == iterEnd �̸� ���̾���.
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
		client->SetMyRoomNum(NoneRoom);
		client->InitBetting(); // ���� �ʱ�ȭ ��Ŵ
		client->SaveCalculateMoney(); // ���� ������ ������..�ؾ���
		(*myRoomIter).get()->EraseClient(shared_clientInfo);
		return myRoomIter;
	}
	return mRooms.end();
}

int CRoomManager::MakeRoom(const string & roomName, const int & channelNumber, const int & battingMoney)
{
	RoomPtr newRoom(new CRoom(newRoomNumber, channelNumber, roomName, battingMoney));
	PushRoom(newRoom);
	return newRoomNumber;
}

bool CRoomManager::EnterRoom(const LinkPtr & shared_clientInfo, int targetRoomNumBer)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return false;
	if (true == client->IsRoomEnterState()) // �̹� �濡 �ִ��� Ȯ��
		return false;
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), targetRoomNumBer);
	if (mRooms.end() != targetRoomIter)
	{
		int BattingMoney = (*targetRoomIter)->GetBattingMoney();
		if (BattingMoney <= client->GetMyMoney())
		{
			(*targetRoomIter)->PushClient(shared_clientInfo, targetRoomNumBer);
			return true;
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
		return ((*targetRoomIter)->IsAllReady());
	}
	return false;
}

bool CRoomManager::IsAllReadyBatting(const LinkPtr & shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return false;
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != targetRoomIter)
	{
		return ((*targetRoomIter)->IsAllReadyBetting());
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
		int winnerPK = -1;	bool isDraw = false;	// �¸��� ��ȣ, ��� ����
		if (true == (*myRoomIter).get()->BattingResult(winnerPK, isDraw))
		{
			(*myRoomIter).get()->AllCalculateMoney();
			(*myRoomIter).get()->AllInitBetting();

		}
		else
		{
			if (true == isDraw)
			{
				(*myRoomIter).get()->AllInitBetting();
				(*myRoomIter).get()->AllRefundBettingMoney();
			}
		}
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



