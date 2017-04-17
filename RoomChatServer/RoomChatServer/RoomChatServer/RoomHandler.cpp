#include "RoomHandler.h"
#include"Link.h"
#include"Room.h"
#include"ConstEnumInfo.h"

CRoomHandler::CRoomHandler()
{
}


CRoomHandler::~CRoomHandler()
{
}

bool CRoomHandler::exitRoom(CLink* clientInfo, CRoomManager* roomManager)
{
#pragma region 나가고자 하는 방 전달자(iter)찾기
	int channelNum = clientInfo->getMyChannelNum();
	int roomNum = clientInfo->getMyRoomNum();
	if (roomNum == NoneRoom)
		return false;
	RoomListIt myRoomIter = roomManager->getMyRoomIter(channelNum, roomNum); // 나가고자하는 방 전달자
#pragma endregion
	// 만약 없는 방이면
	if (&myRoomIter == nullptr)
		return false;
	CRoom* currentRoom = *myRoomIter;
	cout << currentRoom->getRoomName() << " 방을 나갑니다." << endl;
#pragma region 방안에 클라이언트 찾아서 erase시키기
	LinkListIt iterBegin = currentRoom->getIterMyInfoBegin();
	LinkListIt iterEnd = currentRoom->getIterMyInfoEnd();
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		CLink* client = (*iterBegin);
		if (client == clientInfo)
		{
			client->setMyRoomNum(NoneRoom);
			iterBegin = currentRoom->eraseClient(iterBegin); // 원래 있던 방에서 빼기	
			if (0 == currentRoom->getAmountPeople())
			{
				cout << currentRoom->getRoomName() << " 방에 아무도 없습니다." << endl;
				// 방 리스트에서 삭제 시키고
				roomManager->eraseRoom(myRoomIter);
				delete currentRoom;
			}
			break;
		}
	}
#pragma endregion

	return true;
}

bool CRoomHandler::makeRoom(CLink* clientInfo, CRoomManager* roomManager, char* roomName)
{
	if (clientInfo->getMyRoomNum() != NoneRoom)
	{
		cout << "방에 들어와 있음" << endl;
		return false;
	}
	int myChannelNum = clientInfo->getMyChannelNum();
	CRoom* newRoom = nullptr;
	int roomNum = 0;

	if (roomManager->isRoomListEmpty())
	{
		roomNum = 0;
	}
	else 
	{
		roomNum = roomManager->getEmptyRoomNum();
	}
	newRoom = new CRoom(roomNum, myChannelNum, roomName);
	cout << roomNum << " 번으로 방 만듬" << endl;
	// 새로운 room 만듬
	roomManager->pushRoom(newRoom);
	// 새로운 room에 소켓 넣어줌
	newRoom->pushClient(clientInfo);
	clientInfo->setMyRoomNum(roomNum);
	cout << "방 만들기 성공" << endl;
	return true;
}

bool CRoomHandler::enterRoom(CLink* clientInfo, CRoomManager* roomManager, int targetRoomNo)
{
	RoomListIt iterBegin = roomManager->getIterRoomBegin();
	RoomListIt iterEnd = roomManager->getIterRoomEnd();
	// 들어가고자 하는 번호의 room이 있나?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		CRoom* curRoom = (*iterBegin);
		if (targetRoomNo == curRoom->getRoomNum())
		{
			curRoom->pushClient(clientInfo);// 방에 넣어주기
			clientInfo->setMyRoomNum(targetRoomNo);
			return true; // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}
	cout << "없는 방 입니다." << endl;
	return false;
}

char * CRoomHandler::returnRoomName(char * message)
{
	*message++; // 'm' or 'ㅣ' or 'c' 다음 포인터로 옮김
	int i = 0;
	char* roomName = new char[30]; //return할 방 이름
	while (*message != '\0') // null 아닐때까지
	{
		roomName[i] = *message;
		*message++;
		i++;
	}
	roomName[i] = '\0';
	return roomName;
}


/*
// 안쓰고있는 가장 큰 수
CRoom* temp = *iterBegin;
int roomLastNum = temp->getRoomNum();
// room번호 저장할 공간
int roomNum = 0; // 0부터 시작
// 방 번호로 쓰고 있지 않는 가장 작은 숫자 찾기
for (; iterBegin != iterEnd; ++iterBegin)
{
CRoom* curRoom = *iterBegin; // (*iterBegin)-> 안된다 왜지??
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
cout << "쓰고 있지 않는 숫자 발견 = " << roomNum << endl;
break;
}
}
*/