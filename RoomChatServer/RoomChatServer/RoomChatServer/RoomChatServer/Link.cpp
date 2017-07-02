#include"Link.h"
#include"RoomHandler.h"
#include"GaChar.h"
CLink::CLink(SOCKET& clientSocket, string strPKNumber, char* name) :
	mClientSocket(clientSocket),
	mMyChannelNum(0),
	mMyRoomNum(NoneRoom),
	mMyMoney(StartMoney),
	mMyPKNumber(stoi(strPKNumber))
{
	size_t length = strlen(name) + 1;
	mName = new char[length];
	strcpy_s(mName, length, name);
	mMyCards.reserve(CardKind);
}


CLink::~CLink()
{
	cout << mName << "클라이언트 정보가 삭제 됩니다. = " << endl;
	delete[] mName;
	closesocket(mClientSocket);
	cout << "클라이언트 삭제 완료" << endl;
}

void CLink::EmptyCard()
{
	ScopeLock<MUTEX> MU(mRAII_LinkMUTEX);
	mMyCards.clear();
}

void CLink::InitCard(Card * card, int amount, int exp, int evol, int star)
{
	ScopeLock<MUTEX> MU(mRAII_LinkMUTEX);
	shared_ptr<Card> newCard_(card);
	MyCardInfo* cardInfo = new MyCardInfo(newCard_, amount, exp, evol, star);
	shared_ptr<MyCardInfo> newCard(cardInfo);
	mMyCards.push_back(newCard);
}
