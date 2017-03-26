#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET* clientSocket):ClientSocket(clientSocket), Name(new char[NameSize]), MyChannelNum(0), MyRoomNum(NoneRoom), MS(new MessageStruct())
{
	//	Name = "abc"; 값으로 초기화 하면 나중에 바꿀 수가 없다. 왜지?
}


CLink::~CLink()
{
	cout << Name << "클라이언트 정보가 삭제 됩니다." << endl;
	delete[] Name;// 
	closesocket(*ClientSocket);
	delete MS;
}
