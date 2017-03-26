#include "RoomHandler.h"
#include"Link.h"
#include"Room.h"

CRoomHandler::CRoomHandler()
{
}


CRoomHandler::~CRoomHandler()
{
}

bool CRoomHandler::exitRoom(CLink* clientInfo, CRoomChannelManager* roomChannelManager)
{
#pragma region �������� �ϴ� �� ������(iter)ã��
	int channelNum = clientInfo->getMyChannelNum();
	int roomNum = clientInfo->getMyRoomNum();
	if (roomNum == NoneRoom)
		return false;
	RoomListIt myRoomIter = roomChannelManager->getMyRoomIter(channelNum, roomNum); // ���������ϴ� �� ������
#pragma endregion
	// ���� ���� ���̸�
	if (&myRoomIter == nullptr)
		return false;
	CRoom* currentRoom = *myRoomIter;
	cout << currentRoom->getRoomName() << " ���� �����ϴ�." << endl;
#pragma region ��ȿ� Ŭ���̾�Ʈ ã�Ƽ� erase��Ű��
	LinkListIt iterBegin = currentRoom->getIterMyInfoBegin();
	LinkListIt iterEnd = currentRoom->getIterMyInfoEnd();
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		CLink* client = (*iterBegin);
		if (client == clientInfo)
		{
			client->setMyRoomNum(NoneRoom);
			iterBegin = currentRoom->eraseClient(iterBegin); // ���� �ִ� �濡�� ����	
			if (0 == currentRoom->getAmountPeople())
			{
				cout << currentRoom->getRoomName() << " �濡 �ƹ��� �����ϴ�." << endl;
				// �� ����Ʈ���� ���� ��Ű��
				roomChannelManager->eraseRoom(myRoomIter);
				delete currentRoom;
			}
			break;
		}
	}
#pragma endregion

	return true;
}

bool CRoomHandler::makeRoom(CLink* clientInfo, CRoomChannelManager* roomChannelManager, char* roomName)
{
	if (clientInfo->getMyRoomNum() != NoneRoom)
	{
		cout << "�濡 ���� ����" << endl;
		return false;
	}
	int myChannelNum = clientInfo->getMyChannelNum();
	CRoom* newRoom = nullptr;
	int roomNum = 0;

	if (roomChannelManager->isRoomListEmpty())
	{
		roomNum = 0;
	}
	else 
	{
		roomNum = (*roomChannelManager->getIterRoomBegin())->getRoomNum() + 1;
	}
	newRoom = new CRoom(roomNum, myChannelNum, roomName);
	cout << roomNum << " ������ �� ����" << endl;
	// ���ο� room ����
	roomChannelManager->pushRoom(newRoom);
	// ���ο� room�� ���� �־���
	newRoom->pushClient(clientInfo);
	clientInfo->setMyRoomNum(roomNum);
	cout << "�� ����� ����" << endl;
	return true;
}

bool CRoomHandler::enterRoom(CLink* clientInfo, CRoomChannelManager* roomChannelManager, int targetRoomNo)
{
	RoomListIt iterBegin = roomChannelManager->getIterRoomBegin();
	RoomListIt iterEnd = roomChannelManager->getIterRoomEnd();
	// ������ �ϴ� ��ȣ�� room�� �ֳ�?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		CRoom* curRoom = (*iterBegin);
		if (targetRoomNo == curRoom->getRoomNum())
		{
			curRoom->pushClient(clientInfo);// �濡 �־��ֱ�
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
	char* roomName = new char[30]; //return�� �� �̸�
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