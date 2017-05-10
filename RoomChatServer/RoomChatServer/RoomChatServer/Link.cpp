#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET& clientSocket, string name_):
	ClientSocket(clientSocket),
	MyChannelNum(0), 
	MyRoomNum(NoneRoom)
{
	g_NameMS.set_message(name_);
}


CLink::~CLink()
{
	cout << g_NameMS.message() << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�." << endl;
	closesocket(ClientSocket);
}
