#include"Link.h"
#include"RoomHandler.h"

CLink::CLink(SOCKET& clientSocket, string name_, int clientNum):
	ClientSocket(clientSocket),
	MyChannelNum(0), 
	MyRoomNum(NoneRoom)
{
	g_MS.set_message("New");
	g_NameMS.set_message(name_);
	int nameSize = g_NameMS.ByteSize();
	mNameSerialize = new char[nameSize];
	g_NameMS.SerializeToArray(mNameSerialize, nameSize);

	mNameData.set_clientnum(clientNum);
	mNameData.set_size(nameSize);
	mNameData.set_type(g_DataType::MESSAGE);
}


CLink::~CLink()
{
	cout << g_NameMS.message() << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�." << endl;
	delete[] mNameSerialize;
	closesocket(ClientSocket);
}
