#include "RoomHandlere.h"
#include"Link.h"
#include"Room.h"

CRoomHandlere::CRoomHandlere()
{
}


CRoomHandlere::~CRoomHandlere()
{
}

bool CRoomHandlere::exitRoom(CMyInfo * clientInfo, CLink* link)
{
#pragma region �������� �ϴ� �� ������(iter)ã��
	int channelNum = clientInfo->getMyChannelNum();
	int roomNum = clientInfo->getMyRoomNum();
	if (roomNum == NoneRoom)
		return false;
	RoomListIt myRoomIter = link->getMyRoomIter(channelNum, roomNum);
#pragma endregion
	// ���� ���� ���̸�
	if (&myRoomIter == nullptr)
		return false;
	CRoom* currentRoom = *myRoomIter;
	cout << currentRoom->getRoomName() << " ���� �����ϴ�." << endl;
#pragma region ��ȿ� ���� ã�Ƽ� erase��Ű��
	SocketListIt iterBegin = currentRoom->getIterSocketBegin();
	SocketListIt iterEnd = currentRoom->getIterSocketEnd();
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin) == clientInfo->getClientSocket())
		{
			clientInfo->setMyRoomNum(NoneRoom);
			iterBegin = currentRoom->eraseSocket(iterBegin); // ���� �ִ� �濡�� ����	
			//link->getMyChannel(channelNum)->pushSocket(*iterBegin); // �ٽ� ä�η� �̵�
			if (0 == currentRoom->getAmountPeople())
			{
				cout << currentRoom->getRoomName() << " �濡 �ƹ��� �����ϴ�." << endl;
				// �� ����Ʈ���� ���� ��Ű��
				link->eraseRoom(myRoomIter);
				delete currentRoom;
			}
			break;
		}
	}
#pragma endregion

	return true;
}

bool CRoomHandlere::makeRoom(CMyInfo* clientInfo, CLink* link, char * roomName)
{
	if (clientInfo->getMyRoomNum() != NoneRoom)
	{
		cout << "�濡 ���� ����" << endl;
		return false;
	}
	int myChannelNum = clientInfo->getMyChannelNum();
	CRoom* newRoom;
	int roomNum;

	if (link->isRoomListEmpty())
	{
		roomNum = 0;
	}
	else 
	{
		roomNum = (*link->getIterRoomBegin())->getRoomNum() + 1;
	}
	newRoom = new CRoom(roomNum, myChannelNum, roomName);
	cout << roomNum << " ������ �� ����" << endl;
	// ���ο� room ����
	link->pushRoom(newRoom);
	// ���ο� room�� ���� �־���
	newRoom->pushSocket(clientInfo->getClientSocket());
	clientInfo->setMyRoomNum(roomNum);
	cout << "�� ����� ����" << endl;
	return true;
}

bool CRoomHandlere::enterRoom(CMyInfo* clientInfo, CLink* link, int targetRoomNo)
{
	RoomListIt iterBegin = link->getIterRoomBegin();
	RoomListIt iterEnd = link->getIterRoomEnd();
	// ������ �ϴ� ��ȣ�� room�� �ֳ�?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		CRoom* curRoom = (*iterBegin);
		if (targetRoomNo == curRoom->getRoomNum())
		{
			curRoom->pushSocket(clientInfo->getClientSocket());// �濡 �־��ֱ�
			clientInfo->setMyRoomNum(targetRoomNo);
			return true; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	cout << "���� �� �Դϴ�." << endl;
	return false;
}

char * CRoomHandlere::returnRoomName(char * message)
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