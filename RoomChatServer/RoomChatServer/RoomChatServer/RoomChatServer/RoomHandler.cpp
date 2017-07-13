#include "RoomHandler.h"
#include"ErrorHandler.h"
#include"Link.h"
#include"Room.h"
//#include"ConstEnumInfo.h"

CRoomHandler::CRoomHandler()
{
}


CRoomHandler::~CRoomHandler()
{
}

bool CRoomHandler::ExitRoom(CLink* clientInfo, CRoomManager* roomManager)
{
#pragma region �������� �ϴ� �� ������(iter)ã��
	int channelNum = clientInfo->GetMyChannelNum();
	int roomNum = clientInfo->GetMyRoomNum();
	if (roomNum == NoneRoom)
		return false;
	RoomListIt myRoomIter = roomManager->GetMyRoomIter(channelNum, roomNum); // ���������ϴ� �� ������
#pragma endregion
	// ���� ���� ���̸�
	if (myRoomIter == roomManager->GetIterRoomEnd())
		return false;
	if (0 >= (*myRoomIter).use_count())
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_ROOM_COUNT_ZORO, clientInfo);
		return false;
	}
	CRoom* currentRoom = (*myRoomIter).get();
	cout << currentRoom->GetRoomName() << " ���� �����ϴ�." << endl;
#pragma region ��ȿ� Ŭ���̾�Ʈ ã�Ƽ� erase��Ű��
	LinkListIt iterBegin = currentRoom->GetIterMyInfoBegin();
	LinkListIt iterEnd = currentRoom->GetIterMyInfoEnd();
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		CLink* client = (*iterBegin).get();
		if (client == clientInfo)
		{
			client->SetMyRoomNum(NoneRoom);
			iterBegin = currentRoom->EraseClient(iterBegin); // ���� �ִ� �濡�� ����	
			if (0 == currentRoom->GetAmountPeople())
			{
				cout << currentRoom->GetRoomName() << " �濡 �ƹ��� �����ϴ�." << endl;
				// �� ����Ʈ���� ���� ��Ű��
				roomManager->EraseRoom(myRoomIter);
			}
			return true;
		}
	}
#pragma endregion

	return false;
}

bool CRoomHandler::MakeRoom(shared_ptr<CLink> shared_clientInfo, CRoomManager* roomManager, const string& roomName)
{
	CLink* clientInfo = nullptr;
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
	}
	else
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
		return false;
	}

	if (clientInfo->GetMyRoomNum() != NoneRoom)
	{
		cout << "�濡 ���� ����" << endl;
		return false;
	}
	int myChannelNum = clientInfo->GetMyChannelNum();
	int roomNum = 0;

	if (roomManager->IsRoomListEmpty())
	{
		roomNum = 0;
	}
	else 
	{
		roomNum = roomManager->GetEmptyRoomNum();
	}
	// ���ο� room ����
	shared_ptr<CRoom> newRoom(new CRoom(roomNum, myChannelNum, roomName));
	cout << roomNum << " ������ �� ����" << endl;
	roomManager->PushRoom(newRoom);
	// ���ο� room�� ���� �־���
	newRoom.get()->PushClient(shared_clientInfo);
	clientInfo->SetMyRoomNum(roomNum);
	cout << "�� ����� ����" << endl;
	return true;
}

bool CRoomHandler::EnterRoom(shared_ptr<CLink> shared_clientInfo, CRoomManager* roomManager, int targetRoomNo)
{
	CLink* clientInfo = nullptr;
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
	}
	else
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
		return false;
	}

	RoomListIt iterBegin = roomManager->GetIterRoomBegin();
	RoomListIt iterEnd = roomManager->GetIterRoomEnd();
	// ������ �ϴ� ��ȣ�� room�� �ֳ�?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin).use_count() < 0)
		{
			ErrorHandStatic->ErrorHandler(ERROR_SHARED_ROOM_COUNT_ZORO, clientInfo);
			continue;
		}
		CRoom* curRoom = (*iterBegin).get();
		if (targetRoomNo == curRoom->GetRoomNum())
		{
			curRoom->PushClient(shared_clientInfo);// �濡 �־��ֱ�
			clientInfo->SetMyRoomNum(targetRoomNo);
			return true; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	cout << "���� �� �Դϴ�." << endl;
	return false;
}

//char * CRoomHandler::returnRoomName(char * message)
//{
//	*message++; // 'm' or '��' or 'c' ���� �����ͷ� �ű�
//	int i = 0;
//	char* roomName = new char[NameSize]; //return�� �� �̸�
//	while (*message != '\0') // null �ƴҶ�����
//	{
//		roomName[i] = *message;
//		*message++;
//		i++;
//	}
//	roomName[i] = '\0';
//	return roomName;
//}


/*
// �Ⱦ����ִ� ���� ū ��
CRoom* temp = *iterBegin;
int roomLastNum = temp->getRoomNum();
// room��ȣ ������ ����
int roomNum = 0; // 0���� ����
// �� ��ȣ�� ���� ���� �ʴ� ���� ���� ���� ã��
for (; iterBegin != iterEnd; ++iterBegin)
{
CRoom* curRoom = *iterBegin; // (*iterBegin)-> �ȵȴ� ����??
++iterBegin;
CRoom* nextRoom = *iterBegin;
if (nextRoom == nullptr)
{
roomNum = roomLastNum;
break;
}
if (curRoom->getRoomNum() == nextRoom->getRoomNum() + 1)
{
--iterBegin;
continue;
}
else
{
roomNum = nextRoom->getRoomNum() + 1;
cout << "���� ���� �ʴ� ���� �߰� = " << roomNum << endl;
break;
}
}
*/