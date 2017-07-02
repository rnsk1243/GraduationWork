#include"Link.h"
#include"RoomHandler.h"
#include"GaChar.h"
CLink::CLink(SOCKET& clientSocket, string strPKNumber, char* name_) :
	ClientSocket(clientSocket),
	MyChannelNum(0),
	MyRoomNum(NoneRoom),
	MyMoney(StartMoney),
	MyPKNumber(stoi(strPKNumber))
{
	size_t length = strlen(name_) + 1;
	Name = new char[length];
	strcpy_s(Name, length, name_);
	mMyCards.reserve(CardKind);
}


CLink::~CLink()
{
	cout << Name << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�. = " << endl;
	delete[] Name;
	closesocket(ClientSocket);
	cout << "Ŭ���̾�Ʈ ���� �Ϸ�" << endl;
}

void CLink::EmptyCard()
{
	ScopeLock<MUTEX> MU(RAII_LinkMUTEX);
	mMyCards.clear();
}

void CLink::initCard(Card * card, int amount, int exp, int evol, int star)
{
	ScopeLock<MUTEX> MU(RAII_LinkMUTEX);
	shared_ptr<Card> newCard_(card);
	MyCardInfo* cardInfo = new MyCardInfo(newCard_, amount, exp, evol, star);
	shared_ptr<MyCardInfo> newCard(cardInfo);
	mMyCards.push_back(newCard);
}
