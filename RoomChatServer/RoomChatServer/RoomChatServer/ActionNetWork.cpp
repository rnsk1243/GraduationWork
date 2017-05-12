#include "ActionNetWork.h"
#include "ChannelManager.h"
#include"RoomManager.h"
#include"ReadHandler.h"
#include"graduationWork.pb.h"
using namespace graduationWork;


CActionNetWork::CActionNetWork()
{
}


CActionNetWork::~CActionNetWork()
{
	
}

int CActionNetWork::sendSize(SOCKET & sock, int size, g_DataType type, int flags)
{
	g_DataSize g_size(size, type);
	char temp[BufSize];
	int byteSize = g_size.ByteSize();
	cout << "DataSize ũ�� = " << byteSize << endl;
	bool isSerialize = g_size.SerializeToArray(temp, byteSize);
	if (!isSerialize)
		return SerializeFail;

	int sendSize = 0;
	while (true)
	{
		sendSize += send(sock, temp, byteSize, flags);
		cout << "���� ũ��0 = " << sendSize << endl;
		if (sendSize >= byteSize)
			break;
	}
	cout << "���� ũ�� = " << sendSize << endl;
	return sendSize;
}

int CActionNetWork::recvSize(SOCKET & sock, g_DataSize& g_size, int flags)
{
	// DataSize �ʱ�ȭ
	g_size.Clear();
	char temp[BufSize];
	int size = 0;
	size += recv(sock, temp, recvSizeByteSize, flags);
	if (size == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return SOCKET_ERROR;
	}
bool isParseSucces = false;
	if (size > 0)
	{
		isParseSucces = g_size.ParseFromArray(temp, size);
		if (!isParseSucces)
		{
			cout << "parse ����	" << endl;
			return ParseFail;
		}
	}
	else if(0 == size)
	{
		cout << "���� ũ�Ⱑ ����" << endl;
		return SOCKET_ERROR;
	}

	return g_size.size();
}

int CActionNetWork::recvn(SOCKET & socket, g_Message& g_MS, int flags)
{
	char tempMessage[BufSize];
	g_DataSize g_size;
	int left = recvSize(socket, g_size);
#pragma endregion
#pragma region �޼��� �ޱ�
	int isSuccess = 0;
	while (left > 0)
	{
		isSuccess += recv(socket, tempMessage, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	cout << "---IsInitialized? = " << g_MS.IsInitialized() << endl;
	cout << "���� ũ�� = " << isSuccess << endl;
	bool isParseSucces = false;
	if (isSuccess > 0)
	{
		isParseSucces = g_MS.ParseFromArray(tempMessage, isSuccess);
		if (!isParseSucces)
		{
			cout << "parse ����	" << endl;
			return ParseFail;
		}
	}
	else if(0 == isSuccess)
	{
		cout << "���� ũ�� = " << isSuccess << endl;
		return ERROR_RECV_ZERO;
	}
	
#pragma endregion
	cout << "���� idPw�޽��� = " << g_MS.message() << endl;
//	g_MS.set_messagesize(isSuccess);
	return SuccesRecv;
}

int CActionNetWork::sendn(CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags)
{
	char temp[BufSize];
#pragma region �����޽���, ä��&�� ��ȣ ������
	g_Message& g_MS = clientInfo.getMessage();
	if (&g_MS.message() == nullptr)
		return OccuredError;
	
	int size = g_MS.ByteSize();
	int channelNum = clientInfo.getMyChannelNum();
	int roomNum = clientInfo.getMyRoomNum();

	bool isSerializeSucces = g_MS.SerializeToArray(temp, size);
	if (!isSerializeSucces)
		return SerializeFail;
#pragma endregion
#pragma region ������ �� ���� �� ���� �ӽ� ����
	LinkListIt iterBegin;
	LinkListIt iterEnd;

	CChannel* myChannel = nullptr;
	CRoom* myRoom = nullptr;
#pragma endregion

#pragma region ���� ���� ä�� �� ��������
	if (roomNum == NoneRoom)
	{
		myChannel =  channelManager.getMyChannel(channelNum);
		if (myChannel == nullptr)
		{
			cout << "���� ä�� ��ü�� ã�� �� �����ϴ�" << endl;
			return OccuredError;
		}
		// ä�� ���� ����Ʈ iter
		iterBegin = myChannel->getIterMyInfoBegin();
		iterEnd = myChannel->getIterMyInfoEnd();
	}
	else
	{
		CRoom* myRoom = *(roomManager.getMyRoomIter(channelNum, roomNum));
		// �� ���� ����Ʈ iter
		if (myRoom == nullptr)
		{
			cout << "���� �水ü�� ã�� �� �����ϴ�" << endl;
			return OccuredError;
		}
		iterBegin = myRoom->getIterMyInfoBegin();
		iterEnd = myRoom->getIterMyInfoEnd();
	}
#pragma endregion
#pragma region ���� ���� ä�� or �濡�� ������
	// �濡 �ִ� ��� ������� ������
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		size_t curDataSize = 0;
		// �޽��� ���� �ڽ��̸�
		if ((*iterBegin) == &clientInfo)
		{
			continue; // ������ �ʰ� ���
		}

		SOCKET& clientSocket = (*iterBegin)->getClientSocket();
		sendMyName(clientSocket, clientInfo); // �̸� ������
		sendSize(clientSocket, size, g_DataType::MESSAGE);
		while (true)
		{
			curDataSize += send(clientSocket, temp, size, flags);
			if (curDataSize >= size)
				break;
		}
	}
#pragma endregion
	//cout << "�Ϸ�" << endl;
	return SuccesSend;
}

int CActionNetWork::sendn(SOCKET & socket, g_Message& g_MS, int flags)
{
	char message[BufSize];
	int size = g_MS.ByteSize();
	g_MS.SerializeToArray(message, size);
	sendSize(socket, size, g_DataType::MESSAGE);// ������ ������
	size_t temp = 0;
	while (true)
	{
		temp += send(socket, message, size, flags);
		if (temp >= size)
			break;
	}
	return SuccesSend;
}

int CActionNetWork::recvn(CLink& clientInfo, CCommandController& commandController, int flags)
{
#pragma region ���� ������ ũ�� ��������
	
	SOCKET& clientSocket = clientInfo.getClientSocket();
	g_Message& g_MS = clientInfo.getMessage();

	g_DataSize g_dataSize;
	int left = recvSize(clientSocket, g_dataSize);
	if(SOCKET_ERROR == left)
		return SOCKET_ERROR;

	switch (g_dataSize.type())
	{
	case g_DataType::TRANSFORM:
		recvnTransform(clientSocket, clientInfo, left);
		break;
	case g_DataType::MESSAGE:

		break;
	default:
		break;
	}

#pragma endregion
#pragma region �޼��� �ޱ�
	
//	char* charMessage = g_MS.message().c_str();
#pragma endregion
//#pragma region ��ɸ޽��� �̸� ó��
//	char* ptr = strchr(g_MS.message(), '/');
//	if (ptr != nullptr)
//	{
//		return commandController.commandHandling(clientInfo, ptr);
//	}
//#pragma endregion
	//cout << "���� �޽��� = " << g_MS.message() << endl;
	return SuccesRecv;
}
int CActionNetWork::recvnTransform(SOCKET & clientSocket, CLink & clientInfo, int recvSize, int flags)
{
	int isSuccess = 0;
	char recvBuffer[BufSize];
	g_Transform& mTransform = clientInfo.getTransform();
	const int resultRecvSize = recvSize; // �޾ƾ߸� �ϴ� ũ��
	while (resultRecvSize > 0)
	{
		isSuccess += recv(clientSocket, recvBuffer, resultRecvSize, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= resultRecvSize)
			break;
	}
	//cout << "IsInitialized? = " << mTransform.IsInitialized() << endl;
	bool isParseSucces = false;
	if (isSuccess > 0)
	{
		isParseSucces = mTransform.ParseFromArray(recvBuffer, isSuccess);
		if (!isParseSucces)
		{
			cout << "parse ����	" << endl;
			return ParseFail;
		}
	}
	else if (0 == isSuccess)
	{
		cout << "���� ũ�Ⱑ 0 �̴�. " << endl;
		return ERROR_RECV_ZERO;
	}
	cout << "��Ŷ ��ȣ = " << mTransform.packetnum() << "�� // ";
	cout << "Transform.position = ( " << 
		mTransform.position().x() << " , " <<
		mTransform.position().y() << " , " <<
		mTransform.position().z() << " , " <<
		" )" << endl;
	return isSuccess;
}
int CActionNetWork::recvnMessage(SOCKET & clientSocket, CLink & clientInfo, int recvSize, int flags)
{
	//int isSuccess = 0;
	//while (recvSize > 0)
	//{
	//	isSuccess += recv(clientSocket, message, left, flags);
	//	//cout << "success = " << isSuccess << endl;
	//	if (isSuccess == SOCKET_ERROR)
	//	{
	//		cout << "2recvn ERROR" << endl;
	//		return SOCKET_ERROR;
	//	}
	//	else if (isSuccess >= left)
	//		break;
	//}
	//cout << "IsInitialized? = " << g_MS.IsInitialized() << endl;
	//bool isParseSucces = false;
	//if (isSuccess > 0)
	//{
	//	isParseSucces = g_MS.ParseFromArray(message, isSuccess);
	//	if (!isParseSucces)
	//	{
	//		cout << "parse ����	" << endl;
	//		return ParseFail;
	//	}
	//}
	//else if (0 == isSuccess)
	//{
	//	cout << "���� ũ�� = " << isSuccess << endl;
	//	return ERROR_RECV_ZERO;
	//}
	return 0;
}

int CActionNetWork::sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags)
{

	g_Message& myName = clientInfo.getMyNameMessage();
	int sendRecvSize = myName.ByteSize();
	
	sendSize(clientSocket, sendRecvSize, g_DataType::MESSAGE);

	char myNameMessage[NameSize];
	myName.SerializeToArray(myNameMessage, sendRecvSize);
	int temp = 0;
	while (true)
	{
		temp += send(clientSocket, myNameMessage, sendRecvSize, flags);
		if (temp >= sendRecvSize)
			break;
	}
	return temp;
}


