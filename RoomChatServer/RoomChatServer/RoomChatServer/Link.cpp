#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET* clientSocket):ClientSocket(clientSocket), MyChannelNum(0), MyRoomNum(NoneRoom), MS(new MessageStruct()){}


CLink::~CLink()
{
	cout << "클라이언트 정보가 삭제 됩니다." << endl;
	closesocket(*ClientSocket);
	delete MS;
}
