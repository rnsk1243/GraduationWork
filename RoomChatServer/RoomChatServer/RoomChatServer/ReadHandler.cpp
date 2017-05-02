#include "ReadHandler.h"
#include"ConstEnumInfo.h"
#include"ActionNetWork.h"

CReadHandler::CReadHandler()
{
}


CReadHandler::~CReadHandler()
{
}

bool CReadHandler::Search(const char * textFileName, bool isFullMatch, int count, ...)
{
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "파일이 없습니다." << endl;
		return false;
	}
	va_list Marker;
	va_start(Marker, count);

	vector<string> searchTarget;
	for (int i = 0; i < count; i++)
	{
		searchTarget.push_back(va_arg(Marker, string));
	}

	int sameAmount = 0;
	while (!inFile.eof())
	{
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
				return false;
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
		return true;
	}
	return false;
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
