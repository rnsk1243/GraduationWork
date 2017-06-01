#include "Room.h"



g_CreateCharaterInfo * CRoom::newCharaterInfo(CLink * client)
{
	g_CreateCharaterInfo* returnVal = new g_CreateCharaterInfo();
	returnVal->set_pknum(client->getMyPKNumber());
	returnVal->set_servant(client->getMyServentNumber());
	returnVal->set_team(client->getMyTeam());
	return returnVal;
}

CRoom::CRoom(int roomNum, int channelNum, char* roomName) :
	RoomNum(roomNum),
	ChannelNum(channelNum),
	RoomName(roomName),
	AmountPeople(0)
{
	
	InitializeCriticalSection(&CS_MyInfoList);
}


CRoom::~CRoom()
{
	if (ClientInfos.empty())
	{
		cout << RoomNum << " �� ���� ���� �˴ϴ�." << endl;
		delete RoomName;
		LinkListIt begin = getIterMyInfoBegin();
		LinkListIt end = getIterMyInfoEnd();
		for (; begin != end; ++begin)
		{
			delete(*begin);
		}
		ClientInfos.clear();
		DeleteCriticalSection(&CS_MyInfoList);
		cout << "�� ���� �Ϸ�" << endl;
	}else
	{
		cout << "�� ���� ����" << endl;
	}
}
// ��� �غ� �Ǿ����� ��ȯ
bool CRoom::updatePlayerReadyInfo()
{
	bool isAllReady = true;
	if (EnterRoomPeopleLimit != AmountPeople)
	{
		isAllReady = false;
	}
	LinkListIt iterBegin = ClientInfos.begin();
	LinkListIt iterEnd = ClientInfos.end();
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		setReadyPlayerInfo(*iterBegin);
		if ((*iterBegin)->getMyTeam() == NoneTeam)
		{
			isAllReady = false;
		}
		if ((*iterBegin)->getMyServentNumber() == servantNone)
		{
			isAllReady = false;
		}
	}
	return isAllReady;
}

bool CRoom::isTeamBalance()
{
	if (EnterRoomPeopleLimit != AmountPeople)
	{
		return false;
	}
	LinkListIt iterBegin = ClientInfos.begin();
	LinkListIt iterEnd = ClientInfos.end();
	int redTeamAmount = 0;
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->getMyTeam() == RedTeam)
		{
			redTeamAmount++;
		}
	}
	if (2 == redTeamAmount)
	{
		return true;
	}
	return false;
}

void CRoom::setReadyPlayerInfo(CLink * client)
{
	switch (client->getMyPKNumber())
	{
	case 1:
		mReadySet.set_allocated_player1(newCharaterInfo(client));
		break;
	case 2:
		mReadySet.set_allocated_player2(newCharaterInfo(client));
		break;
	case 3:
		mReadySet.set_allocated_player3(newCharaterInfo(client));
		break;
	case 4:
		mReadySet.set_allocated_player4(newCharaterInfo(client));
		break;
	default:
		break;
	}
}

