#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET& clientSocket, char* name_):
	ClientSocket(clientSocket),
	MyChannelNum(0), 
	MyRoomNum(NoneRoom)
{
	size_t length = strlen(name_) + 1;
	Name = new char[length];
	strcpy_s(Name, length, name_);
}


CLink::~CLink()
{
	cout << Name << "클라이언트 정보가 삭제 됩니다. = " << endl;
	delete[] Name;
	closesocket(ClientSocket);
	cout << "클라이언트 삭제 완료" << endl;
}
