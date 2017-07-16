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
	// �� ȥ�� �̸� ������ false;
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
	// �� ȥ�� �̸� ������ false;
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
	// �� ȥ�� �̸� ������ false;
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

