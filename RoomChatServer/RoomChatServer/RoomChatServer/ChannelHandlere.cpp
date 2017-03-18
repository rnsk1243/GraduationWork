#include "ChannelHandlere.h"



CChannelHandlere::CChannelHandlere()
{
}


CChannelHandlere::~CChannelHandlere()
{
}

bool CChannelHandlere::enterChannel(CMyInfo* clientInfo, CLink* link, int targetChannelNo)
{
	// channel리스트 iterator
	ChannelListIt iterBegin = link->getIterChannelBegin();
	ChannelListIt iterEnd = link->getIterChannelEnd();
	// 옮기고자 하는 번호의 Channel 포인터 얻기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->getChannelNum())
		{
			cout << targetChannelNo << "번 채널로 이동 합니다." << endl;
			(*iterBegin)->pushSocket(clientInfo->getClientSocket()); // 채널에 넣어주기
			clientInfo->setMyChannelNum(targetChannelNo);
			return true; // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}
	cout << "채널 옮기기 실패" << endl;
	return false;
}

bool CChannelHandlere::exitChannel(CMyInfo* clientInfo, CLink* link)
{
	CChannel* myChannel = link->getMyChannel(clientInfo->getMyChannelNum());
	cout << myChannel->getChannelNum() << "번 채널을 나갑니다." << endl;

	if (myChannel != nullptr)
	{
		SocketListIt iterBegin = myChannel->getIterBegin();
		SocketListIt iterEnd = myChannel->getIterEnd();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin) == clientInfo->getClientSocket())
			{
				iterBegin = myChannel->eraseSocket(iterBegin); // 원래 있던 방에서 빼기
				break;
			}
		}
		return true;
	}
	else
	{
		cout << "채널 나가기 실패" << endl;
		return false;
	}
}

bool CChannelHandlere::pushChannel(CChannel * newChannel)
{
	return false;
}
