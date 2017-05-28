#include "WriteHandler.h"
#include"ReadHandler.h"


CWriteHandler::CWriteHandler()
{
}

CWriteHandler * CWriteHandler::getInstance()
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

bool CWriteHandler::write(const char * textFileName, int count, ...)
{
	ofstream outFile(textFileName, ios::app);
	if (!outFile)
	{
		cout << "������ �����ϴ�." << endl;
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

//int CWriteHandler::writeGacharResult(const char * textFileName, const char * userName)
//{
//	string userNameString = userName;
//	// ã�� ���̵� ���� ���
//	int targetLine = ReadHandlerStatic->Search(textFileName, false, 1, userNameString);
//
//
//
//	return 0;
//}
