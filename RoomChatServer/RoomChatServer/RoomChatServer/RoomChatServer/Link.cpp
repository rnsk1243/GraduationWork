#include"Link.h"
#include"RoomHandler.h"
#include"GaChar.h"
CLink::CLink(SOCKET& clientSocket, char* name_) :
	ClientSocket(clientSocket),
	MyChannelNum(0),
	MyRoomNum(NoneRoom),
	MyMoney(100)
{
	size_t length = strlen(name_) + 1;
	Name = new char[length];
	strcpy_s(Name, length, name_);
}


CLink::~CLink()
{
	cout << Name << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�. = " << endl;
	delete[] Name;
	closesocket(ClientSocket);
	cout << "Ŭ���̾�Ʈ ���� �Ϸ�" << endl;
}

void CLink::pushCard(shared_ptr<Card>& card)
{
	mMyCards.push_back(card);
	MyMoney -= CardCost;
}
