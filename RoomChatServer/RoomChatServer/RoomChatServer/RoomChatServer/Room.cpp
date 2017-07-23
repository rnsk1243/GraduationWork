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
	cout << mRoomNum << " �� ���� ���� �˴ϴ�." << endl;
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
	//	cout << "�� ���� ����" << endl;
	//}
	cout << "�� ���� �Ϸ�" << endl;
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

bool CRoom::IsAllReady()
{
	// �ο����� ���ڸ��� ������ false;
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
	// �ο����� ���ڸ��� ������ false;
	if (EnterRoomPeopleLimit > mAmountPeople)
	{
		ErrorHandStatic->ErrorHandler(ERROR_BATTING_RESULT_ALONE);
		return false;
	}
	LinkListIt iterBegin = GetIterMyInfoBegin();
	CLink* winner = (*iterBegin).get();
	int curWinnerStat = CardStatic->GetCardStat(winner->GetMyBattingCardNumber()); // ���� ���� ���� ���� ũ��
	bool isDraw = true; // �ְ� ���� ������ �� ����� �� �̻��ΰ�? 
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
				// ���
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
	bool isSaveResult = true; // ��� .txt�� ���� �Ǿ���? �� ���̶� �ȵǸ� false ��ȯ
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


// ��� ī�� �³�?
bool CRoom::IsAllReadyBetting()
{
	// �ο����� ���ڸ��� ������ false;
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
	// ��� ī�� ��
	return true;
}

