#include "ReadHandler.h"
//#include"ConstEnumInfo.h"
#include"ActionNetWork.h"
#include"Util.h"

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

int CReadHandler::Search(const char * textFileName, vector<string>& tempUserInfo, int count, ...)
{
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "������ �����ϴ�." << endl;
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
		vector<string> userInfoVec = Parse(tempString, '|');

		vector<string>::iterator iterUserInfoBegin = userInfoVec.begin();
		vector<string>::iterator iterTargetBegin = searchTarget.begin();
		for (; iterUserInfoBegin != userInfoVec.end(); ++iterUserInfoBegin)
		{
			for (; iterTargetBegin != searchTarget.end(); ++iterTargetBegin)
			{
				int compareResult = iterTargetBegin->compare(*iterUserInfoBegin);
				if (0 == compareResult)
				{
					sameAmount++;
				}
			}
			iterTargetBegin = searchTarget.begin();
		}
		if (sameAmount == count)
		{
			tempUserInfo = userInfoVec;
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

const string CReadHandler::GetLastLine(const string & textFileName)
{
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "������ �����ϴ�." << endl;
	}
	inFile.seekg(-2, ios::end);
	char checkLine = ' ';
	while (checkLine != '\n')
	{
		inFile.seekg(-1, ios::cur);
		checkLine = (char)inFile.peek();
		//		cout << checkLine << endl;
	}
	inFile.seekg(2, ios::cur);
	string lastLine;
	getline(inFile, lastLine);

	inFile.close();
	return lastLine;
}


bool CReadHandler::ReadUserCardLine(const string& textFileName, const int& userPKNum, vector<string>& targetTemp)
{
	string strUserName = IntToString(userPKNum);
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "������ �����ϴ�." << endl;
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


bool CReadHandler::ReadUserCard(CLink* client, const string& textFileName)
{
	vector<string> userCardInfo;
	Card* pushCard;
	if (!ReadUserCardLine(textFileName, client->GetMyPKNumber(), userCardInfo))
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
	// Card list ������
	CardListIt CardBegin = CardStatic->getCardListIterBegin();
	CardListIt CardEnd = CardStatic->getCardListIterEnd();

	++userCardInfoIterBegin; // ���̵� �κ� �ѱ�
	for (; userCardInfoIterBegin != userCardInfoIterEnd; ++userCardInfoIterBegin)
	{
		// (ī���ȣ/ī�尹��)�� '/'�� �������� �ڸ�
		vector<string> invenBox = ReadHandlerStatic->Parse((*userCardInfoIterBegin), '/');
		//cout << "ī���ȣ = " << invenBox[0].c_str() << endl;
		//cout << "ī�尹�� = " << invenBox[1].c_str() << endl;
		if (0 == invenBox[1].compare(CardEmpty))
		{
			cout << "�ѱ�!!!!!!!!!!!!!!!!!!!!" << endl;
			continue;
		}
		else
		{
			int myCardNum = stoi(invenBox[0]);
			int myCardAmount = stoi(invenBox[1]);
			// exp ������ ��
			for (; CardBegin != CardEnd; ++CardBegin)
			{
				if (((*CardBegin).get())->cardNum == myCardNum)
				{
					pushCard = (*CardBegin).get();
					cout << "�߰� �� ī�� = " << pushCard->name << endl;
					client->initCard(pushCard, myCardAmount);
					break;
				}
			}
		}
	}
	return true;
}

const string CReadHandler::GetNextUserNum(const string & textFileName)
{
	//string lastLine = GetLastLine(textFileName);
	//vector<string> parseStr = Parse(lastLine, '|');
	//return parseStr[0];
	cout << "=============userNumFile Read!===========" << endl;
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "������ �����ϴ�." << endl;
	}
	string nextUserNum;
	getline(inFile, nextUserNum);
	inFile.close();
	return nextUserNum;
}
