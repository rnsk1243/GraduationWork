#include "MyInfo.h"
#include"Link.h"
#include"RoomHandlere.h"

CMyInfo::CMyInfo(SOCKET* clientSocket):ClientSocket(clientSocket), MyChannelNum(0), MyRoomNum(NoneRoom)
{
	MS = new MessageStruct();
}


CMyInfo::~CMyInfo()
{
	cout << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�." << endl;
	closesocket(*ClientSocket);
	delete MS;
}
