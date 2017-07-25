#include "WriteHandler.h"
#include"ReadHandler.h"
#include"ConstEnumInfo.h"
#include"ErrorHandler.h"
#include"Util.h"

CWriteHandler::CWriteHandler()
{
}

CWriteHandler * CWriteHandler::GetInstance()
{
	if (nullptr == WriteHandlerStatic)
	{
		WriteHandlerStatic = new CWriteHandler();
	}
	return WriteHandlerStatic;
}


CWriteHandler::~CWriteHandler()
{
}

bool CWriteHandler::Write(const char * textFileName, int count, ...)
{
	ofstream outFile(textFileName, ios::app);
	if (!outFile)
	{
		cout << "파일이 없습니다." << endl;
		return false;
	}
	va_list Marker;
	va_start(Marker, count);

	for (int i = 0; i < count; i++)
	{
		outFile << va_arg(Marker, string).c_str() << '|';
	}
	outFile << '\n';
	outFile.close();
	va_end(Marker);
	return true;
}

bool CWriteHandler::Write(const char * textFileName, const vector<string>& strWrite)
{
	ofstream outFile(textFileName, ios::app);
	if (!outFile)
	{
		cout << "파일이 없습니다." << endl;
		return false;
	}
	vector<string>::const_iterator iterBeginWrite = strWrite.begin();
	for (; iterBeginWrite != strWrite.end(); ++iterBeginWrite)
	{
		outFile << (*iterBeginWrite).c_str() << '|';
	}
	outFile << '\n';
	outFile.close();
	return true;
}



//int CWriteHandler::writeGacharResult(const char * textFileName, const char * userName)
//{
//	string userNameString = userName;
//	// 찾는 아이디 라인 취득
//	int targetLine = ReadHandlerStatic->Search(textFileName, 1, userNameString);
//
//
//
//	return 0;
//}

//수정할유저가 있는 라인 까지 커서 이동해야할 크기 반환(text이름, 해당 유저가 기록된 라인 수, 유저가 기록된 라인을 저장할 공간)
//int CWriteHandler::BeginToTargetUserLineCurserMoveSize(const char* textName, int targetUserLineNum, char* targetLine)
//{
//	ifstream input(textName);
//	input.seekg(0, ios::beg);
//	char checkLine;
//	int offset = 0;
//	int line = 1;
//	while (true)
//	{
//		input.seekg(1, ios::cur);
//		offset++;
//		checkLine = (char)input.peek();
//		if (checkLine == '\n')
//		{
//			input.seekg(1, ios::cur);
//			line++;
//			offset++;
//		}
//		if (targetUserLineNum == line)
//		{
//			input.seekg(1, ios::cur);
//			offset++;
//			break;
//		}
//	}
//	input.getline(targetLine, BufSize);
//	input.close();
//
//	return offset;
//}
//
//// 수정할 유저가 있는 라인부터 유저가 있는 곳까지 커서가 이동해야할 크기 반환(유저가기록된라인, 찾는카드숫자, 새로운카드인지확인용변수, 찾은카드 갯수보관변순)
//int CWriteHandler::TargetLineToUserCurserMoveSize(const char* targetSouce, string& searchCardNum, bool& isNewCard, int& cardAmount)
//{
//	int offset = 0;
//	string targetLine = targetSouce;
//	// 해당 아이디 라인을 '|'을 기준으로 자름
//	vector<string> tempVec = ReadHandlerStatic->Parse(targetLine, '|');
//
//	vector<string>::iterator iterBegin = tempVec.begin();
//	vector<string>::iterator iterEnd = tempVec.end();
//	// 맨 앞 id 부분을 더함  
//	offset += (strlen((*iterBegin).c_str()) + 1/*'|'때문에 1을 더함*/);
//	++iterBegin; // 아이디 부분 넘김
//	for (; iterBegin != iterEnd; ++iterBegin) // 카드 부분
//	{
//		// (카드번호/카드갯수)을 '/'을 기준으로 자름
//		vector<string> tempVec2 = ReadHandlerStatic->Parse((*iterBegin), '/');
//		vector<string>::iterator iterBegin2 = tempVec2.begin(); // 카드 번호 부분
//																// 카드 번호와 찾으려는 카드 일치 여부 확인
//		int compareResult = (*iterBegin2).compare(searchCardNum);
//		if (0 == compareResult)
//		{	// 일치하면 offset리턴(가지고 있던 카드임)
//			isNewCard = false;
//			++iterBegin2; // 카드 갯수 부분
//			cardAmount = atoi((*iterBegin2).c_str());
//			return offset;
//		}
//		// 일치 하지 않으면 "카드번호/카드갯수|" <-이 만큼을 offset에 더함
//		offset += (strlen((*iterBegin).c_str()) + 1);
//	}
//	// 신규 카드
//	isNewCard = true;
//	cardAmount = 0;
//	return offset;
//}

// 예외처리 완료된 함수
// 카드 기록(기록할텍스트이름, 수정할곳까지 이동해야하는 수, 갯수수정할카드, 기록할숫자, 자릿수)
bool CWriteHandler::WriteObj(const string& textName, int offset, int recordedNumber, int cipher)
{
	char charObj[20];
	if (false == IntToAlphabet(recordedNumber, cipher, charObj))
	{
		return false;
	}
	string strObj(charObj);

	ofstream output(textName, ios::out | ios::in);

	output.seekp(offset, ios::beg);

	const char* doWrite = strObj.c_str();
	cout << "기록 될 문자열 = " << doWrite << endl;
	output.write(doWrite, strlen(doWrite));
	output.close();
	return true;
	/*
	중간에 삽입이 불가능 하다고.. 파일 새로 만들고 모든 데이터를 복사하는 도중에 삽입해야 합니다.
	You cannot insert in the middle of the file.
	You have to copy the old file to a new file and insert whatever you want in the middle during copying to the new file.
	Otherwise, if you intend to overwrite data/lines in the existing file, that is possible by using
	std::ostream::seekp() to identify the position within the file.

	https://stackoverflow.com/questions/30642827/how-to-write-to-middle-of-a-file-in-c
	*/

	//if (isNewCard)
	//{// 새로운 카드
	//	output.seekp(-7, ios::cur); // "xxxxxx" 부분 만큼 뒤로(수정될 부분)
	//	string nextSpace = "xxxxxx"; // 다음 새로운 카드 공간 확보용
	//	result += nextSpace;
	//	const char* doWrite = result.c_str();
	//	output.write(doWrite, strlen(doWrite));
	//}
	//else
	//{// 새로운 카드가 아님
	//	const char* doWrite = result.c_str();
	//	output.write(doWrite, strlen(doWrite));
	//}
}

bool CWriteHandler::WriteNextJoinUserNum(const string& textFileName,const int& nextUserNum)
{
	ofstream outFile(textFileName, ios::beg);
	if (!outFile)
	{
		cout << "파일이 없습니다." << endl;
		return false;
	}

	int lastUserNum = nextUserNum;
	++lastUserNum;
	string userNum = IntToString(lastUserNum);
	outFile << userNum;
	outFile.close();
	return true;
}

// 
