#include "RoomHandler.h"
#include"Link.h"
#include"Room.h"
//#include"ConstEnumInfo.h"
#include"ErrorHandler.h"

CRoomHandler::CRoomHandler()
{
}


CRoomHandler::~CRoomHandler()
{
}

bool CRoomHandler::exitRoom(CLink* clientInfo, CRoomManager* roomManager)
{
#pragma region �������� �ϴ� �� ������(iter)ã��
	int channelNum = clientInfo->getMyChannelNum();
	int roomNum = clientInfo->getMyRoomNum();
	if (roomNum == NoneRoom)
		return false;
	RoomListIt myRoomIter = roomManager->getMyRoomIter(channelNum, roomNum); // ���������ϴ� �� ������
#pragma endregion
	// ���� ���� ���̸�
	if (myRoomIter == roomManager->getIterRoomEnd())
		return false;
	if (0 >= (*myRoomIter).use_count())
	{
		CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
		return false;
	}
	CRoom* currentRoom = (*myRoomIter).get();
	cout << currentRoom->getRoomName() << " ���� �����ϴ�." << endl;
#pragma region ��ȿ� Ŭ���̾�Ʈ ã�Ƽ� erase��Ű��
	LinkListIt iterBegin = currentRoom->getIterMyInfoBegin();
	LinkListIt iterEnd = currentRoom->getIterMyInfoEnd();
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		CLink* client = (*iterBegin).get();
		if (client == clientInfo)
		{
			client->setMyRoomNum(NoneRoom);
			iterBegin = currentRoom->eraseClient(iterBegin); // ���� �ִ� �濡�� ����	
			if (0 == currentRoom->getAmountPeople())
			{
				cout << currentRoom->getRoomName() << " �濡 �ƹ��� �����ϴ�." << endl;
				// �� ����Ʈ���� ���� ��Ű��
				roomManager->eraseRoom(myRoomIter);
			}
			return true;
		}
	}
#pragma endregion

	return false;
}

bool CRoomHandler::makeRoom(shared_ptr<CLink> shared_clientInfo, CRoomManager* roomManager, char* roomName)
{
	CLink* clientInfo = nullptr;
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
	}
	else
	{
		CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
		return false;
	}

	if (clientInfo->getMyRoomNum() != NoneRoom)
	{
		cout << "�濡 ���� ����" << endl;
		return false;
	}
	int myChannelNum = clientInfo->getMyChannelNum();
	int roomNum = 0;

	if (roomManager->isRoomListEmpty())
	{
		roomNum = 0;
	}
	else 
	{
		roomNum = roomManager->getEmptyRoomNum();
	}
	// ���ο� room ����
	shared_ptr<CRoom> newRoom(new CRoom(roomNum, myChannelNum, roomName));
	cout << roomNum << " ������ �� ����" << endl;
	roomManager->pushRoom(newRoom);
	// ���ο� room�� ���� �־���
	newRoom.get()->pushClient(shared_clientInfo);
	clientInfo->setMyRoomNum(roomNum);
	cout << "�� ����� ����" << endl;
	return true;
}

bool CRoomHandler::enterRoom(shared_ptr<CLink> shared_clientInfo, CRoomManager* roomManager, int targetRoomNo)
{
	CLink* clientInfo = nullptr;
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
	}
	else
	{
		CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
		return false;
	}

	RoomListIt iterBegin = roomManager->getIterRoomBegin();
	RoomListIt iterEnd = roomManager->getIterRoomEnd();
	// ������ �ϴ� ��ȣ�� room�� �ֳ�?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin).use_count() < 0)
		{
			CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
			continue;
		}
		CRoom* curRoom = (*iterBegin).get();
		if (targetRoomNo == curRoom->getRoomNum())
		{
			curRoom->pushClient(shared_clientInfo);// �濡 �־��ֱ�
			clientInfo->setMyRoomNum(targetRoomNo);
			return true; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	cout << "���� �� �Դϴ�." << endl;
	return false;
}

char * CRoomHandler::returnRoomName(char * message)
{
	*message++; // 'm' or '��' or 'c' ���� �����ͷ� �ű�
	int i = 0;
	char* roomName = new char[NameSize]; //return�� �� �̸�
	while (*message != '\0') // null �ƴҶ�����
	{
		roomName[i] = *message;
		*message++;
		i++;
	}
	roomName[i] = '\0';
	return roomName;
}


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