#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET& clientSocket, char* name_):
	ClientSocket(clientSocket),
	Name(name_), 
	MyChannelNum(0), 
	MyRoomNum(NoneRoom)
{
}


CLink::~CLink()
{
	cout << Name << "클라이언트 정보가 삭제 됩니다." << endl;
	delete[] Name;
	closesocket(ClientSocket);
}
