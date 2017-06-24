#include "ReadHandler.h"
//#include"ConstEnumInfo.h"
#include"ActionNetWork.h"

CReadHandler::CReadHandler()
{
}

CReadHandler * CReadHandler::getInstance()
{
	if (nullptr == ReadHandlerStatic)
	{
		ReadHandlerStatic = new CReadHandler();
	}
	return ReadHandlerStatic;
}


CReadHandler::~CReadHandler()
{
}

int CReadHandler::Search(const char * textFileName, bool isFullMatch, int count, ...)
{
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "파일이 없습니다." << endl;
		return 0;
	}
	va_list Marker;
	va_start(Marker, count);

	vector<string> searchTarget;
	for (int i = 0; i < count; i++)
	{
		searchTarget.push_back(va_arg(Marker, string));
	}

	int sameAmount = 0;
	int line = 0;
	while (!inFile.eof())
	{
		line++;
		sameAmount = 0;
		char temp[IdPwSize];
		inFile.getline(temp, IdPwSize);
		string tempString = temp;
		vector<string> tempVec = Parse(tempString, '|');

		if (isFullMatch)
		{
			if (searchTarget.size() != tempVec.size())
			{
				cout << "갯수가 일치하지 않습니다." << endl;
				return 0;
			}
		}

		for (int i = 0; i < count; i++)
		{
			if (count > (int)tempVec.size()) // tempVec 범위 벗어나는것 방지
				break;
			int compareResult = searchTarget[i].compare(tempVec[i]);
			if (0 == compareResult)
			{
				sameAmount++;
			}
			else
			{
				break;
			}
		}
		if (sameAmount == count)
		{
			break;
		}
	}
	va_end(Marker);
	inFile.close();
	if (sameAmount == count)
	{
		return line;
	}
	return 0;
}

vector<string> CReadHandler::Parse(const string & str, const char & ch)
{
	string next;
	vector<string> result;

	for (string::const_iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == ch)
		{
			if (!next.empty())
			{
				result.push_back(next);
				next.clear();
			}
		}
		else
		{
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}


bool CReadHandler::ReadUserCardLine(const string textFileName, const char * userName, vector<string>& targetTemp)
{
	string strUserName = userName;
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "파일이 없습니다." << endl;
		return false;
	}

	while (!inFile.eof())
	{
		char temp[IdPwSize];
		inFile.getline(temp, IdPwSize);
		string tempString = temp;
		vector<string> tempVec = Parse(tempString, '|');

		for (int i = 0; i < (int)tempVec.size(); i++)
		{
			int compareResult = strUserName.compare(tempVec[i]);
			if (0 == compareResult)
			{
				targetTemp = tempVec;
				return true;
			}
			else
			{
				break;
			}
		}
	}
	inFile.close();
	return false;
}


bool CReadHandler::ReadUserCard(CLink* client, const string textFileName)
{
	vector<string> userCardInfo;
	Card* pushCard;
	if (!ReadUserCardLine(textFileName, client->getMyName(), userCardInfo))
	{
		CErrorHandler::ErrorHandler(ERROR_INIT_USER_CARD);
		return false;
	}
	if (!client->isEmptyCard())
	{
		client->EmptyCard();		
	}
	vector<string>::iterator userCardInfoIterBegin = userCardInfo.begin();
	vector<string>::iterator userCardInfoIterEnd = userCardInfo.end();
	// Card list 가져옴
	CardListIt CardBegin = CardStatic->getCardListIterBegin();
	CardListIt CardEnd = CardStatic->getCardListIterEnd();

	++userCardInfoIterBegin; // 아이디 부분 넘김
	for (; userCardInfoIterBegin != userCardInfoIterEnd; ++userCardInfoIterBegin)
	{
		// (카드번호/카드갯수)을 '/'을 기준으로 자름
		vector<string> invenBox = ReadHandlerStatic->Parse((*userCardInfoIterBegin), '/');
		cout << "카드번호 = " << invenBox[0].c_str() << endl;
		cout << "카드갯수 = " << invenBox[1].c_str() << endl;
		if (0 == invenBox[1].compare(CardEmpty))
		{
			cout << "넘김!!!!!!!!!!!!!!!!!!!!" << endl;
			continue;
		}
		else
		{
			int myCardNum = stoi(invenBox[0]);
			int myCardAmount = stoi(invenBox[1]);
			// exp 가져올 곳
			for (; CardBegin != CardEnd; ++CardBegin)
			{
				if (((*CardBegin).get())->cardNum == myCardNum)
				{
					pushCard = (*CardBegin).get();
					cout << "추가 한 카드 = " << pushCard->name << endl;
					client->initCard(pushCard, myCardAmount);
					break;
				}
			}
		}
	}
	return true;
}
