#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET& clientSocket, string name_, int clientNum):
	ClientSocket(clientSocket),
	MyChannelNum(0), 
	MyRoomNum(NoneRoom),
	mMyPKNumber(clientNum),
	mMyServentNumber(servantNone),
	mMyTeam(NoneTeam)
{
	g_MS.set_message("New");
	g_NameMS.set_message(name_);
	int nameSize = g_NameMS.ByteSize();
	mNameSerialize = new char[nameSize];
	g_NameMS.SerializeToArray(mNameSerialize, nameSize);
	
	mNameData.set_clientnum(mMyPKNumber);
	mNameData.set_size(nameSize);
	mNameData.set_type(g_DataType::MESSAGE);
}


CLink::~CLink()
{
	cout << g_NameMS.message() << "클라이언트 정보가 삭제 됩니다." << endl;
	delete[] mNameSerialize;
	closesocket(ClientSocket);
}
