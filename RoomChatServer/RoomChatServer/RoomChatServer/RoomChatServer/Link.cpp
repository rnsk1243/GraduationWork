#include"Link.h"
#include"RoomHandler.h"
#include"ErrorHandler.h"

CLink::CLink(SOCKET& clientSocket, string strPKNumber, char* name) :
	mClientSocket(clientSocket),
	mMyChannelNum(0),
	mMyRoomNum(NoneRoom),
	mMyPKNumber(stoi(strPKNumber)),
	mIsInitCards(false),
	mIsInitGoods(false)
{
	size_t length = strlen(name) + 1;
	mName = new char[length];
	strcpy_s(mName, length, name);
	mMyCards.reserve(CardKind);
}


CLink::~CLink()
{
	cout << mName << "Ŭ���̾�Ʈ ������ ���� �˴ϴ�. = " << endl;
	delete[] mName;
	closesocket(mClientSocket);

	cout << "Ŭ���̾�Ʈ ���� �Ϸ�" << endl;
}

void CLink::EmptyCard()
{
	ScopeLock<MUTEX> MU(mRAII_LinkMUTEX);
	mMyCards.clear();
}

void CLink::InitCard(Card * card, int amount, int exp, int evol, int star)
{
	if (true == mIsInitCards) // �ʱ�ȭ ������ �ִٸ� ����
	{
		ErrorHandStatic->ErrorHandler(ERROR_INIT_CARD_TRUE, this);
		return;
	}
	ScopeLock<MUTEX> MU(mRAII_LinkMUTEX);
	shared_ptr<Card> newCard_(card);
	MyCardInfo* cardInfo = new MyCardInfo(newCard_, amount, exp, evol, star);
	shared_ptr<MyCardInfo> newCard(cardInfo);
	mMyCards.push_back(newCard);
}

bool CLink::InitGoods(int initMoney)
{
	if (true == mIsInitGoods)
	{
		ErrorHandStatic->ErrorHandler(ERROR_INIT_GOODS_TRUE, this);
		return false;
	}
	return InitMoney(initMoney);
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