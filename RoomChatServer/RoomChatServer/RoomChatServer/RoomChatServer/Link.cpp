#include"Link.h"
#include"RoomHandler.h"
#include"ErrorHandler.h"

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

bool MyCardInfo::SetExp(int addExp, int & resultExp)
{
	if (1 == mIsEvolution)
	{
		ErrorHandStatic->ErrorHandler(ERROR_COMPOSE_EVOUTION_CARD);
		return false;
	}
	mExp += addExp;
	if (100 <= mExp)
	{
		mExp = 0;
		resultExp = mExp;
		mIsEvolution = 1;
		return true;
	}
	resultExp = mExp;
	return true;
}