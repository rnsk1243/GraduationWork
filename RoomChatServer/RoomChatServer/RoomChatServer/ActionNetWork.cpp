#include "ActionNetWork.h"
#include "ChannelManager.h"
#include"RoomManager.h"
#include"ReadHandler.h"
#include"graduationWork.pb.h"
using namespace graduationWork;
// 시리얼라이즈 데이터 받기
int CActionNetWork::recvDataSource(SOCKET & recvOwnSocket, g_DataSize& g_dataSize, char * destination, int flags)
{
	int isSuccess = 0;
	const int resultRecvSize = recvSize(recvOwnSocket, g_dataSize); // 받아야만 하는 크기
	if (resultRecvSize == ERROR_PARSE)
	{
		return ERROR_PARSE;
	}
	//cout << "resultRecvSize = " << resultRecvSize << endl;
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
// 데이터 얼마나 받아야하나?
int CActionNetWork::recvSize(SOCKET & sock, g_DataSize& g_data, int flags)
{
	// DataSize 초기화
	char temp[BufSize];
	int size = 0;
	//int t = 0;
	while (true)
	{
		//cout << "ㅋsize = "<< size << "] " << t << endl;
		size += recv(sock, temp, RecvSizeByteSize, flags);
		if (size == SOCKET_ERROR)
		{
			CErrorHandler::ErrorHandler(ERROR_RECV);
		}
		if (size >= RecvSizeByteSize)
		{
			break;
		}
		//t++;
	}
	
	bool isParseSucces = false;

	//temp[size] = '\0';
	if (size > 0)
	{
		isParseSucces = g_data.ParseFromArray(temp, size);
		if (!isParseSucces)
		{
			cout << "여기 size = " << size << endl;
			return CErrorHandler::ErrorHandler(ERROR_PARSE);
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
	//cout << "byteSize = " << byteSize << endl;
	//cout << "보낼 데이타 크기 = " << g_data.size() << endl;
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
	cout << "sendSize 보냈다" << sendSize << endl;
	return sendSize;
}





int CActionNetWork::multiSendn(g_DataSize& g_data, LinkListIt iterBegin, LinkListIt iterEnd, CLink & sendOwnLink, LinkInfo* linkInfo, bool isSelf, int flags)
{
	if (!linkInfo->isSerialize)
	{
		return CErrorHandler::ErrorHandler(ERROR_SERIALIZE_TO_ARRAY);
	}
	int size = linkInfo->size;
	char* sendData = linkInfo->sendData;
	// 방에 있는 모든 사람에게 보내기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (!isSelf) // 자신에게 안보낼거니?
		{
			// 메시지 보낸 자신이면
			if ((*iterBegin) == &sendOwnLink)
			{
				//cout << "통과" << endl;
				continue; // 보내지 않고 통과
			}
		}
		
		size_t curDataSize = 0;

		SOCKET& clientSocket = (*iterBegin)->getClientSocket();
		//sendMyName(clientSocket, sendOwnLink); // 이름 보내기
		sendSize(clientSocket, g_data);
		//cout << "보낼 데이터의 position.x ============== " << tt.position().x() << endl;

		while (true)
		{
			curDataSize += send(clientSocket, sendData, size, flags);
			if (curDataSize >= size)
				break;
		}
		//cout << "보낸 데이터 크기 = " << curDataSize << endl;
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
	//cout << "recvn 호출###################################" << endl;
	bool isParseSucces = false;
	if (recvedSize > 0)
	{
		isParseSucces = g_MS.ParseFromArray(recvBuffer, recvedSize);
		if (!isParseSucces)
		{
			cout << "여기인가" << endl;
			return CErrorHandler::ErrorHandler(ERROR_PARSE);
		}
	}
	else if (0 == recvedSize)
	{
		return CErrorHandler::ErrorHandler(ERROR_RECV_ZERO);
	}
#pragma endregion
	cout << "받은 idPw메시지 = " << g_MS.message() << endl;
	return SUCCES_RECV;
}

int CActionNetWork::recvnData(CLink & clientInfo, g_DataSize& g_dataSize, CCommandController& commandController, g_Message& recvResultMessage, int flags)
{
	char recvBuffer[BufSize];
	SOCKET& clientSocket = clientInfo.getClientSocket();
	int isSUCCES = recvDataSource(clientSocket, g_dataSize, recvBuffer); // recvBuffer에 시리얼라이즈된 값 받아 채우기
	if (isSUCCES == ERROR_PARSE)
	{
		return ERROR_PARSE;
	}
	bool isParseSucces = false;
	int recvedSize = g_dataSize.size(); 
	g_DataType type = g_dataSize.type();
	bool isEverySend = false;
	int commandResult = 0;
	if (recvedSize > 0)
	{
		switch (type) // type에 따른 Parse
		{
		case graduationWork::COMMAND:
			isParseSucces = clientInfo.getMessage().ParseFromArray(recvBuffer, recvedSize);
			commandResult = commandController.commandHandling(clientInfo ,clientInfo.getMessage().message(), recvResultMessage);
			if (SUCCES_PLAYER_INFO_LACK == commandResult) // 방 인원수 다 모임. 그리고 준비도 다 됨.
			{
				isEverySend = true;
				g_DataSize g_data;
				g_data.set_type(g_DataType::READYSET);
				sendn(g_data, clientInfo, commandController.getRoomManager(), commandController.getChannelManager(), isEverySend);
			}
			sendnSingle(clientInfo, recvResultMessage, g_DataType::MESSAGE);
		case graduationWork::MESSAGE:
			isParseSucces = clientInfo.getMessage().ParseFromArray(recvBuffer, recvedSize);
			break;
		case graduationWork::TRANSFORM:
			isParseSucces = clientInfo.getTransform().ParseFromArray(recvBuffer, recvedSize);
			//cout << "값 수정 되었나? ====================== " << clientInfo.getTransform().position().x() << endl;
			break;
		default:
			break;
		}
		if (!isParseSucces)
		{
			cout << "여기3?" << endl;
			return CErrorHandler::ErrorHandler(ERROR_PARSE);
		}
	}
	else if (0 == recvedSize)
	{
		return CErrorHandler::ErrorHandler(ERROR_RECV_ZERO);
	}

	//cout << "받은 = (" << clientInfo.getTransform().position().x() << ", " << clientInfo.getTransform().position().y() << ", " << clientInfo.getTransform().position().z() << ")" << endl;
	if (isEverySend)
	{
		return SUCCES_RECV_EVERY_SEND;
	}
	else
	{
		return SUCCES_RECV;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


int CActionNetWork::sendn(g_DataSize& g_data, CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, bool isSelf, int flags)
{
	LinkInfo* linkInfo = nullptr;
	int myRoomNum;
	int myChannelNum;
	int pkClientNum;
	CRoom* myRoom = nullptr;
	switch (g_data.type())
	{
	case g_DataType::TRANSFORM:
	//	cout << "TRANSFORM ---- x = " << clientInfo.getTransform().position().x() << endl;
		linkInfo = &clientInfo.getMyLinkInfoStruct(clientInfo.getTransform());
		break;
	case g_DataType::MESSAGE:
		linkInfo = &clientInfo.getMyLinkInfoStruct(clientInfo.getMessage());
		break;
	case g_DataType::COMMAND:
		return SUCCES_COMMAND;
	case g_DataType::READYSET:
		myRoomNum = clientInfo.getMyRoomNum();
		myChannelNum = clientInfo.getMyChannelNum();
		myRoom = *roomManager.getMyRoomIter(myChannelNum, myRoomNum);
		
		pkClientNum = clientInfo.getMyPKNumber();

		linkInfo = &LinkInfo(myRoomNum, myChannelNum, *myRoom->getReadySet(), g_data, pkClientNum);
		break;
	default:
		return CErrorHandler::ErrorHandler(ERROR_DATA_TYPE_EXCEPTION);
	}
	if (!linkInfo->isSerialize)
	{
		return CErrorHandler::ErrorHandler(ERROR_SERIALIZE_TO_ARRAY);
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
//내가 속한 채널 or 방에게 보내기
	int code = multiSendn(g_data, iterBegin, iterEnd, clientInfo, linkInfo, isSelf);
	return code;
}
// 서버가 담당 클라이언트에게 보내기
//int CActionNetWork::sendn(g_DataSize& g_data, SOCKET & socket, int flags)
//{
//	
//	return sendSize(socket, g_data);
//
//	/*switch (g_data.type())
//	{
//	case g_DataType::TRANSFORM:
//		g_Transform tr;
//		break;
//	case g_DataType::MESSAGE:
//		
//		break;
//	default:
//		break;
//	}
//	
//	int sendSize = 0;
//	while (true)
//	{
//		sendSize += send(socket, temp, byteSize, flags);
//		if (sendSize >= byteSize)
//			break;
//	}
//	return sendSize;*/
//}


int CActionNetWork::sendnSingle(CLink & sendOwnLink, g_Message& g_MS, g_DataType type, int flags)
{
	g_DataSize g_data;
	int myRoomNum = sendOwnLink.getMyRoomNum();
	int myChannelNum = sendOwnLink.getMyChannelNum();
	LinkInfo linkInfo(myRoomNum, myChannelNum, g_MS, g_data, type, sendOwnLink.getMyPKNumber());
	if (!linkInfo.isSerialize)
	{
		return CErrorHandler::ErrorHandler(ERROR_SERIALIZE_TO_ARRAY);
	}
	int size = linkInfo.size;
	char* sendData = linkInfo.sendData;

	size_t curDataSize = 0;

	SOCKET& clientSocket = sendOwnLink.getClientSocket();
	//sendMyName(clientSocket, sendOwnLink); // 이름 보내기
	sendSize(clientSocket, g_data);
	//cout << "보낼 데이터의 position.x ============== " << tt.position().x() << endl;

	g_Message m;
	m.ParseFromArray(sendData, size);
	cout << "g_dataSize = " << g_data.size() << "// 타입 = " << g_data.type() << endl;
	cout << "size = " << size << "  // 보낸 메시지 = " << m.message() << endl;

	while (true)
	{
		curDataSize += send(clientSocket, sendData, size, flags);
		if (curDataSize >= size)
			break;
	}
	cout << "내 메시지 보내기 성공" << endl;
	return SUCCES_SINGLE_SENDN;
}


