#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET* clientSocket):ClientSocket(clientSocket), Name(new char[NameSize]), MyChannelNum(0), MyRoomNum(NoneRoom), MS(new MessageStruct())
{
	//	Name = "abc"; ������ �ʱ�ȭ �ϸ� ���߿� �ٲ� ���� ����. ����?
}


CLink::~CLink()
{
	cout << Name << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�." << endl;
	delete[] Name;// 
	closesocket(*ClientSocket);
	delete MS;
}
