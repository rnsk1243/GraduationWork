#include"Link.h"
#include"RoomHandler.h"
#include"ErrorHandler.h"

CLink::CLink(SOCKET& clientSocket,const string& strPKNumber,const char* name) :
	mClientSocket(clientSocket),
	mMyChannelNum(0),
	mMyRoomNum(NoneRoom),
	mMyPKNumber(stoi(strPKNumber)),
	mIsInitCards(false),
	mIsInitGoods(false),
	mIsGameOK(false),
	mBattingCardNum(NoneCard),
	mMyGoods(stoi(strPKNumber)),
	mDebtMoney(0),
	mPayBackMoney(0)
{
	size_t length = strlen(name) + 1;
	mName = new char[length];
	strcpy_s(mName, length, name);
	mMyCards.reserve(CardKind);
}


CLink::~CLink()
{
	cout << "CLINK 소멸자 호출" << endl;
	SaveCalculateMoney();// 갈땐 가더라도 정산은 하고 가야지?
	cout << mName << "클라이언트 정보가 삭제 됩니다. = " << endl;
	delete[] mName;
	closesocket(mClientSocket);
	delete &mClientSocket;
	cout << "클라이언트 삭제 완료" << endl;
}

bool CLink::IsRoomEnterState()
{
	if (NoneRoom != mMyRoomNum)
	{
		return true;
	}
	return false;
}

bool CLink::PayCardGachar()
{
	if (true == IsMoneyOKGaChar()) 
	{ 
		return MinusMyMoney(CardCost); 
	}
	else
	{
		return false;
	}
}

bool CLink::SetZeroMoney()
{
	EnumErrorCode resultcode;
	if (false == mMyGoods.SetZeroMoney(resultcode))
	{
		ErrorHandStatic->ErrorHandler(resultcode, LinkPtr(this));
		return false;
	}
	return true;
}

void CLink::EmptyCard()
{
	ScopeLock<MUTEX> MU(mRAII_LinkMUTEX);
	mMyCards.clear();
}

bool CLink::InitMoney(int money)
{
	if (false == mMyGoods.InitMoney(money))
	{
		ErrorHandStatic->ErrorHandler(ERROR_INIT_MONEY, LinkPtr(this));
		return false;
	}
	return true;
}

bool CLink::AddMoney(const int & addMoney)
{
	if (0 == addMoney)
	{
		ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY_ZERO, LinkPtr(this));
		return false;
	}
	EnumErrorCode resultcode;
	if (false == mMyGoods.AddMyMoney(addMoney, resultcode))
	{
		ErrorHandStatic->ErrorHandler(resultcode, LinkPtr(this));
		return false;
	}
	return true;
}

bool CLink::MinusMyMoney(const int & minusMoney)
{
	if (0 == minusMoney)
	{
		ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY_ZERO, LinkPtr(this));
		return false;
	}
	EnumErrorCode resultcode;
	if (false == mMyGoods.MinusMyMoney(minusMoney, resultcode))
	{
		ErrorHandStatic->ErrorHandler(resultcode, LinkPtr(this));
		return false;
	}
	return true;
}

bool CLink::IsHaveCard(int cardNum)
{
	MyCardVectorIt myCardIterBegin = GetIterMyCardBegin();
	for (; myCardIterBegin != GetIterMyCardEnd(); ++myCardIterBegin)
	{
		if ((*myCardIterBegin).use_count() > 0)
		{
			CMyCardInfo* myCard = (*myCardIterBegin).get();
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

bool CLink::PayBackMoney(const int & payBack)
{
	// 받을 돈 증가 실제 .txt에 쓰지는 않음 그러나 나중에 이 만큼 증가
	mPayBackMoney += payBack;
	return true;
}

bool CLink::SetMyBattingCard(int cardNum)
{
	if (true == IsHaveCard(cardNum))
	{
		if (NoneCard == mBattingCardNum)
		{
			if (false == FineGamePlayingOut())
			{
				return false;
			}
		}
		mBattingCardNum = cardNum;
		return true;
	}

	return false;
}

bool CLink::GetReadyBatting()
{
	if (NoneCard == mBattingCardNum)
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
bool CLink::GetPrizeBattingMoney(const int bettingMoney)
{
	int prizeMoney = EnterRoomPeopleLimit * bettingMoney; // 총 상금
	PayBackMoney(prizeMoney);
	return true;
}
bool CLink::SaveCalculateMoney()
{
	//정산하고 저장할 돈
	int calculateMoney = 0;
	if (mPayBackMoney == mDebtMoney)
	{
		mPayBackMoney = 0;
		mDebtMoney = 0;
		return true;
	}
	if (mPayBackMoney > mDebtMoney)
	{
		// 받을 돈 - 나갈 돈
		calculateMoney = (mPayBackMoney - mDebtMoney);
		if (true == AddMoney(calculateMoney))
		{
			mPayBackMoney = 0;
			mDebtMoney = 0;
			return true;
		}
	}
	else
	{
		calculateMoney = (mDebtMoney - mPayBackMoney);
		if (true == MinusMyMoney(calculateMoney))
		{
			mPayBackMoney = 0;
			mDebtMoney = 0;
			return true;
		}
	}
	return false;
}

void CLink::InitCard(int cardName, int amount, int exp, int evol, int star)
{
	if (true == mIsInitCards) // 초기화 한적이 있다면 에러
	{
		ErrorHandStatic->ErrorHandler(ERROR_INIT_CARD_TRUE, LinkPtr(this));
		return;
	}
	ScopeLock<MUTEX> MU(mRAII_LinkMUTEX);
	CMyCardInfo* cardInfo = new CMyCardInfo(cardName, amount, exp, evol, star);
	shared_ptr<CMyCardInfo> newCard(cardInfo);
	mMyCards.push_back(newCard);
}

bool CLink::InitGoods(int initMoney)
{
	if (true == mIsInitGoods)
	{
		ErrorHandStatic->ErrorHandler(ERROR_INIT_GOODS_TRUE, LinkPtr(this));
		return false;
	}
	return InitMoney(initMoney);
}

bool CLink::InitBetting()
{
	mIsGameOK = false;
	mBattingCardNum = NoneCard;
	return true;
}

bool CLink::RefundBettingMoney(const int& bettingMoney)
{
	if (NoneCard == mBattingCardNum) // 카드 안냈으면 환불 불필요
	{
		return false;
	}
	if (0 <= (mDebtMoney - bettingMoney))
	{
		mDebtMoney -= bettingMoney;
		return true;
	}
	else
	{
		mDebtMoney = 0;
		return false;
	}
	return false;
}

bool CLink::FineGamePlayingOut()
{
	if (GetMyMoney() >= FineGamePlayingOutMoney) // 빚보다 돈이 더 많으면
	{
		// 돈 감소 (일단 빚으로) 실제 .txt에 쓰지는 않음 그러나 나중에 이 만큼 차감함.
		mDebtMoney += FineGamePlayingOutMoney;
		return true;
	}
	return false;
}


void CLink::SendnMine(const string & message, int flags)
{
	int isSuccess = 0;
	const char* chMessage = message.c_str();
	size_t size = strlen(chMessage);
	isSuccess = send(mClientSocket, (char*)&size, IntSize, flags); // 사이즈 보내기
	if (isSuccess == SOCKET_ERROR)
	{
		//return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND);
	}
	int temp = 0;
	while (true)
	{
		temp += send(mClientSocket, chMessage, (int)size, flags);
		if (temp == SOCKET_ERROR)
		{
			//return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND);
		}
		if (temp >= (int)size)
			break;
	}
}