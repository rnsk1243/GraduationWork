#include "GuestLink.h"


CGuestLink::CGuestLink(const SOCKET & clientSocket):mClientSocket(clientSocket)
{
}

CGuestLink::~CGuestLink()
{
}

const SOCKET CGuestLink::GetClientSocket()
{
	return mClientSocket;
}

void CGuestLink::Sendn(const string & message, int flags)
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
