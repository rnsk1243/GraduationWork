#pragma once
#include"Link.h"
#include"CommandController.h"
//#include"graduationWork.pb.h"

class CActionNetWork
{
	CActionNetWork(const CActionNetWork&);
	CActionNetWork& operator=(const CActionNetWork&);
	// Size 보내기
	int recvDataSource(SOCKET& recvOwnSocket, g_DataSize& g_dataSize, char* destination, int flags = 0); // 시리얼라이즈된 정보 받기, 받아야하는 크기
	int recvSize(SOCKET& sock, g_DataSize& g_data, int flags = 0);
	int sendSize(SOCKET& sock, g_DataSize& g_data, int flags = 0);
	int multiSendn(g_DataSize&  g_data, LinkListIt iterBegin, LinkListIt iterEnd, CLink& sendOwnLink, LinkInfo* linkInfo, int flags = 0);
	// 내 이름 보내기
	int sendMyName(SOCKET& clientSocket, CLink& clientInfo, int flags = 0);
public:
	CActionNetWork();
	~CActionNetWork();
	int recvn(SOCKET& socket, g_Message& g_MS, g_DataSize& g_data, int flags = 0); // 자신에게만 답장 받기
	// 모든 형태 받기
	int recvnData(CLink& clientInfo, g_DataSize& g_dataSize, int flags = 0);
	// 모든 형태 보내기
	int sendn(g_DataSize& g_data, CLink& clientInfo, CRoomManager& roomManager, CChannelManager& channelManager, int flags = 0);
	int sendn(g_DataSize& g_data, SOCKET & socket, int flags = 0); // 자신에게만 보내기
};

