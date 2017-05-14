#include "ActionNetWork.h"
#include "ChannelManager.h"
#include"RoomManager.h"
#include"ReadHandler.h"
#include"graduationWork.pb.h"
using namespace graduationWork;
// �ø�������� ������ �ޱ�
int CActionNetWork::recvDataSource(SOCKET & recvOwnSocket, g_DataSize& g_dataSize, char * destination, int flags)
{
	int isSuccess = 0;
	const int resultRecvSize = recvSize(recvOwnSocket, g_dataSize); // �޾ƾ߸� �ϴ� ũ��
	while (resultRecvSize > 0)
	{
		isSuccess += recv(recvOwnSocket, destination, resultRecvSize, flags);
		if (isSuccess == SOCKET_ERROR)
		{
			return CErrorHandler::ErrorHandler(ERROR_RECV);
		}
		else if (isSuccess >= resultRecvSize)
			break;
	}
	return SUCCES_RECV;
}
// ������ �󸶳� �޾ƾ��ϳ�?
int CActionNetWork::recvSize(SOCKET & sock, g_DataSize& g_data, int flags)
{
	// DataSize �ʱ�ȭ
	g_data.Clear();
	char temp[BufSize];
	int size = 0;
	size += recv(sock, temp, RecvSizeByteSize, flags);
	if (size == SOCKET_ERROR)
	{
		CErrorHandler::ErrorHandler(ERROR_RECV);
	}
	bool isParseSucces = false;
	if (size > 0)
	{
		isParseSucces = g_data.ParseFromArray(temp, size);
		if (!isParseSucces)
		{
			CErrorHandler::ErrorHandler(ERROR_PARSE);
		}
	}
	else if(0 == size)
	{
		CErrorHandler::ErrorHandler(ERROR_RECV_ZERO);
	}
	return g_data.size();
}

int CActionNetWork::sendSize(SOCKET & sock, g_DataSize& g_data, int flags)
{
	char temp[BufSize];
	int byteSize = g_data.ByteSize();

	bool isSerialize = g_data.SerializeToArray(temp, byteSize);
	if (!isSerialize)
		return CErrorHandler::ErrorHandler(ERROR_SERIALIZE_TO_ARRAY);

	int sendSize = 0;
	while (true)
	{
		sendSize += send(sock, temp, byteSize, flags);
		if (sendSize >= byteSize)
			break;
	}
	return sendSize;
}

int CActionNetWork::multiSendn(g_DataSize& g_data, LinkListIt iterBegin, LinkListIt iterEnd, CLink & sendOwnLink, LinkInfo* linkInfo, int flags)
{
	int size = linkInfo->size;
	char* sendData = linkInfo->sendData;
	// �濡 �ִ� ��� ������� ������
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		size_t curDataSize = 0;
		// �޽��� ���� �ڽ��̸�
		if ((*iterBegin) == &sendOwnLink)
		{
			cout << "���" << endl;
			continue; // ������ �ʰ� ���
		}

		SOCKET& clientSocket = (*iterBegin)->getClientSocket();
		//sendMyName(clientSocket, sendOwnLink); // �̸� ������
		sendSize(clientSocket, g_data);
		while (true)
		{
			curDataSize += send(clientSocket, sendData, size, flags);
			if (curDataSize >= size)
				break;
		}
		cout << "���� ������ ũ�� = " << curDataSize << endl;
	}
	return SUCCES_MULTI_SENDN;
}

int CActionNetWork::sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags)
{
	g_DataSize* nameData = clientInfo.getMyNameDataSizeType();
	int recvSize = nameData->size();
	sendSize(clientSocket, *nameData);

	int temp = 0;
	while (true)
	{
		temp += send(clientSocket, clientInfo.getMyNameSerializeData(), recvSize, flags);
		if (temp >= recvSize)
			break;
	}
	return temp;
}

/////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////p	r	i	v	a	t	e///////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

CActionNetWork::CActionNetWork()
{
}

CActionNetWork::~CActionNetWork()
{
	
}

int CActionNetWork::recvn(SOCKET & clientSocket, g_Message& g_MS, g_DataSize& g_data, int flags)
{
	char recvBuffer[BufSize];
	const int recvedSize = recvDataSource(clientSocket, g_data, recvBuffer);
	bool isParseSucces = false;
	if (recvedSize > 0)
	{
		isParseSucces = g_MS.ParseFromArray(recvBuffer, recvedSize);
		if (!isParseSucces)
		{
			return CErrorHandler::ErrorHandler(ERROR_PARSE);
		}
	}
	else if (0 == recvedSize)
	{
		return CErrorHandler::ErrorHandler(ERROR_RECV_ZERO);
	}
#pragma endregion
	cout << "���� idPw�޽��� = " << g_MS.message() << endl;
	return SUCCES_RECV;
}

int CActionNetWork::recvnData(CLink & clientInfo, g_DataSize& g_dataSize, int flags)
{
	char recvBuffer[BufSize];
	SOCKET& clientSocket = clientInfo.getClientSocket();
	recvDataSource(clientSocket, g_dataSize, recvBuffer); // recvBuffer�� �ø��������� �� �޾� ä���
	bool isParseSucces = false;
	int recvedSize = g_dataSize.size(); g_DataType type = g_dataSize.type();
	if (recvedSize > 0)
	{
		switch (type)
		{
		case graduationWork::MESSAGE:
			isParseSucces = clientInfo.getMessage().ParseFromArray(recvBuffer, recvedSize);
			break;
		case graduationWork::TRANSFORM:
			isParseSucces = clientInfo.getTransform().ParseFromArray(recvBuffer, recvedSize);
			break;
		default:
			break;
		}
		if (!isParseSucces)
		{
			return CErrorHandler::ErrorHandler(ERROR_PARSE);
		}
	}
	else if (0 == recvedSize)
	{
		return CErrorHandler::ErrorHandler(ERROR_RECV_ZERO);
	}

	cout << "���� = " << clientInfo.getTransform().position().x() << endl;

	return SUCCES_RECV;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


int CActionNetWork::sendn(g_DataSize& g_data, CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags)
{
	LinkInfo* linkInfo = nullptr;
	switch (g_data.type())
	{
	case g_DataType::TRANSFORM:
		linkInfo = &clientInfo.getMyLinkInfoStruct(clientInfo.getTransform());
		break;
	case g_DataType::MESSAGE:
		linkInfo = &clientInfo.getMyLinkInfoStruct(clientInfo.getMessage());
		break;
	default:
		break;
	}
	LinkListIt iterBegin, iterEnd;
	if (linkInfo == nullptr)
	{
		return CErrorHandler::ErrorHandler(ERROR_LINKINFO_NULL);
	}
	if (linkInfo->myRoomNum == NoneRoom)
	{
		CChannel* myChannel = channelManager.getMyChannel(linkInfo->myChannelNum);
		iterBegin = myChannel->getIterMyInfoBegin();
		iterEnd = myChannel->getIterMyInfoEnd();
	}
	else
	{
		CRoom* myRoom = *(roomManager.getMyRoomIter(linkInfo->myChannelNum, linkInfo->myRoomNum));
		iterBegin = myRoom->getIterMyInfoBegin();
		iterEnd = myRoom->getIterMyInfoEnd();
	}
//���� ���� ä�� or �濡�� ������
	int code = multiSendn(g_data, iterBegin, iterEnd, clientInfo, linkInfo);
	return code;
}
// ������ ��� Ŭ���̾�Ʈ���� ������
int CActionNetWork::sendn(g_DataSize& g_data, SOCKET & socket, int flags)
{
	
	return sendSize(socket, g_data);

	/*switch (g_data.type())
	{
	case g_DataType::TRANSFORM:
		g_Transform tr;
		break;
	case g_DataType::MESSAGE:
		
		break;
	default:
		break;
	}
	
	int sendSize = 0;
	while (true)
	{
		sendSize += send(socket, temp, byteSize, flags);
		if (sendSize >= byteSize)
			break;
	}
	return sendSize;*/
}



