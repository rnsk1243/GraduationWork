#include"Link.h"
#include"RoomHandler.h"
#include"ErrorHandler.h"

CLink::CLink(SOCKET& clientSocket, string strPKNumber, char* name) :
	mClientSocket(clientSocket),
	mMyChannelNum(0),
	mMyRoomNum(NoneRoom),
	mMyPKNumber(stoi(strPKNumber)),
	mIsInitCards(false),
	mIsInitGoods(false),
	mIsGameOK(false),
	mBattingCardNum(-1)
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

bool CLink::PayCardGachar()
{
	if (IsMoneyOKGaChar()) 
	{ 
		return MinusMyMoney(CardCost); 
	}
	else
	{
		return false;
	}
}

void CLink::EmptyCard()
{
	ScopeLock<MUTEX> MU(mRAII_LinkMUTEX);
	mMyCards.clear();
}

bool CLink::IsHaveCard(int cardNum)
{
	MyCardVectorIt myCardIterBegin = GetIterMyCardBegin();
	for (; myCardIterBegin != GetIterMyCardEnd(); ++myCardIterBegin)
	{
		if ((*myCardIterBegin).use_count() > 0)
		{
			MyCardInfo* myCard = (*myCardIterBegin).get();
			if (cardNum == myCard->GetCardNumber())
			{
				if (myCard->GetAmount() > 0)
				{
					return true;
				}
			}
			else
			{
				continue;
			}
		}
	}
	return false;
}

bool CLink::SetMyBattingCard(int cardNum)
{
	if (true == IsHaveCard(cardNum))
	{
		mBattingCardNum = cardNum;
		return true;
	}
	return false;
}

bool CLink::GetReadyBatting()
{
	if (-1 == mBattingCardNum)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int CLink::GetMyBattingCardNumber()
{
	return mBattingCardNum;
}

void CLink::InitCard(int cardName, int amount, int exp, int evol, int star)
{
	if (true == mIsInitCards) // 초기화 한적이 있다면 에러
	{
		ErrorHandStatic->ErrorHandler(ERROR_INIT_CARD_TRUE, this);
		return;
	}
	ScopeLock<MUTEX> MU(mRAII_LinkMUTEX);
	MyCardInfo* cardInfo = new MyCardInfo(cardName, amount, exp, evol, star);
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