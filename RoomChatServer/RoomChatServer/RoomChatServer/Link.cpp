#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET& clientSocket, MessageStruct& MS, int myPKNum):
	ClientSocket(clientSocket),
	Name(new char[NameSize]),
	MyChannelNum(0),
	MyRoomNum(NoneRoom),
	MS(MS),
	MyPKNum(myPKNum)
{
		//Name = "abc";// ������ �ʱ�ȭ �ϸ� ���߿� �ٲ� ���� ����. ����?
}


CLink::~CLink()
{
	cout << Name << "CLink Ŭ���̾�Ʈ ������ ���� �˴ϴ�." << endl;
	delete[] Name;
}
