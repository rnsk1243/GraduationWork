#include "ChannelHandler.h"


CChannelHandler::CChannelHandler()
{
}


CChannelHandler::~CChannelHandler()
{
}

bool CChannelHandler::enterChannel(CLink* clientInfo, CRoomChannelManager* roomChannelManager, int targetChannelNo)
{
	// channel리스트 iterator
	ChannelListIt iterBegin = roomChannelManager->getIterChannelBegin();
	ChannelListIt iterEnd = roomChannelManager->getIterChannelEnd();
	
	// 옮기고자 하는 번호의 Channel 포인터 얻기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->getChannelNum())
		{
			cout << targetChannelNo << "번 채널로 이동 합니다." << endl;
			(*iterBegin)->pushClient(clientInfo); // 채널에 넣어주기
			clientInfo->setMyChannelNum(targetChannelNo);
			return true; // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}
	cout << "채널 옮기기 실패" << endl;
	return false;
}

bool CChannelHandler::exitChannel(CLink* clientInfo, CRoomChannelManager* roomChannelManager)
{
	CChannel* myChannel = roomChannelManager->getMyChannel(clientInfo->getMyChannelNum());
	cout << myChannel->getChannelNum() << "번 채널을 나갑니다." << endl;

	if (myChannel != nullptr)
	{
		LinkListIt iterBegin = myChannel->getIterMyInfoBegin();
		LinkListIt iterEnd = myChannel->getIterMyInfoEnd();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin) == clientInfo)
			{
				iterBegin = myChannel->eraseClient(iterBegin); // 원래 있던 방에서 빼기
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

bool CChannelHandler::pushChannel(CChannel * newChannel)
{
	return false;
}
