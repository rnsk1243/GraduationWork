#include "MyInfo.h"
#include"Link.h"
#include"RoomHandlere.h"

CMyInfo::CMyInfo(SOCKET* clientSocket):ClientSocket(clientSocket), MyChannelNum(0), MyRoomNum(NoneRoom)
{
	MS = new MessageStruct();
}


CMyInfo::~CMyInfo()
{
	cout << "클라이언트 정보가 삭제 됩니다." << endl;
	closesocket(*ClientSocket);
	delete MS;
}
