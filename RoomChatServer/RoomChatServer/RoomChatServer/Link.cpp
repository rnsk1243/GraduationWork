#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET* clientSocket):ClientSocket(clientSocket), MyChannelNum(0), MyRoomNum(NoneRoom), MS(new MessageStruct()){}


CLink::~CLink()
{
	cout << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�." << endl;
	closesocket(*ClientSocket);
	delete MS;
}
