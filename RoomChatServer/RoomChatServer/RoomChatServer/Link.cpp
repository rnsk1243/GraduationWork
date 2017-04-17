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
	cout << Name << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�." << endl;
	delete[] Name;
	closesocket(ClientSocket);
}
