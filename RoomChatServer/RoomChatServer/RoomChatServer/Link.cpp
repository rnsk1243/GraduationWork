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
	cout << g_NameMS.message() << "클라이언트 정보가 삭제 됩니다." << endl;
	closesocket(ClientSocket);
}
