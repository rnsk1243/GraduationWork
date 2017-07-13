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
#pragma region 나가고자 하는 방 전달자(iter)찾기
	int channelNum = clientInfo->GetMyChannelNum();
	int roomNum = clientInfo->GetMyRoomNum();
	if (roomNum == NoneRoom)
		return false;
	RoomListIt myRoomIter = roomManager->GetMyRoomIter(channelNum, roomNum); // 나가고자하는 방 전달자
#pragma endregion
	// 만약 없는 방이면
	if (myRoomIter == roomManager->GetIterRoomEnd())
		return false;
	if (0 >= (*myRoomIter).use_count())
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_ROOM_COUNT_ZORO, clientInfo);
		return false;
	}
	CRoom* currentRoom = (*myRoomIter).get();
	cout << currentRoom->GetRoomName() << " 방을 나갑니다." << endl;
#pragma region 방안에 클라이언트 찾아서 erase시키기
	LinkListIt iterBegin = currentRoom->GetIterMyInfoBegin();
	LinkListIt iterEnd = currentRoom->GetIterMyInfoEnd();
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		CLink* client = (*iterBegin).get();
		if (client == clientInfo)
		{
			client->SetMyRoomNum(NoneRoom);
			iterBegin = currentRoom->EraseClient(iterBegin); // 원래 있던 방에서 빼기	
			if (0 == currentRoom->GetAmountPeople())
			{
				cout << currentRoom->GetRoomName() << " 방에 아무도 없습니다." << endl;
				// 방 리스트에서 삭제 시키고
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
		cout << "방에 들어와 있음" << endl;
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
	// 새로운 room 만듬
	shared_ptr<CRoom> newRoom(new CRoom(roomNum, myChannelNum, roomName));
	cout << roomNum << " 번으로 방 만듬" << endl;
	roomManager->PushRoom(newRoom);
	// 새로운 room에 소켓 넣어줌
	newRoom.get()->PushClient(shared_clientInfo);
	clientInfo->SetMyRoomNum(roomNum);
	cout << "방 만들기 성공" << endl;
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
	// 들어가고자 하는 번호의 room이 있나?
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
			curRoom->PushClient(shared_clientInfo);// 방에 넣어주기
			clientInfo->SetMyRoomNum(targetRoomNo);
			return true; // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}
	cout << "없는 방 입니다." << endl;
	return false;
}

//char * CRoomHandler::returnRoomName(char * message)
//{
//	*message++; // 'm' or 'ㅣ' or 'c' 다음 포인터로 옮김
//	int i = 0;
//	char* roomName = new char[NameSize]; //return할 방 이름
//	while (*message != '\0') // null 아닐때까지
//	{
//		roomName[i] = *message;
//		*message++;
//		i++;
//	}
//	roomName[i] = '\0';
//	return roomName;
//}


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