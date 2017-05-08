#include "ChannelHandler.h"


CChannelHandler::CChannelHandler()
{
}


CChannelHandler::~CChannelHandler()
{
}

bool CChannelHandler::enterChannel(CLink* clientInfo, CChannelManager& channelManager, int targetChannelNo)
{
	// channel리스트 iterator
	ChannelListIt iterBegin = channelManager.getIterChannelBegin();
	ChannelListIt iterEnd = channelManager.getIterChannelEnd();
	
	// 옮기고자 하는 번호의 Channel 포인터 얻기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->getChannelNum())
		{
			cout << targetChannelNo << "번 채널로 이동 합니다." << endl;
			shared_ptr<CLink> pushObj(clientInfo); // 메모리 관리객체에게 넘기고
			(*iterBegin)->pushClient(pushObj); // 채널에 넣어주기
			clientInfo->setMyChannelNum(targetChannelNo);
			return true; // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}
	cout << "채널 옮기기 실패" << endl;
	return false;
}

bool CChannelHandler::exitChannel(CLink& clientInfo, CChannelManager& channelManager)
{
	CChannel* myChannel = channelManager.getMyChannel(clientInfo.getMyChannelNum());
	if (myChannel != nullptr)
	{
		cout << myChannel->getChannelNum() << "번 채널을 나갑니다." << endl;
		LinkListIt iterBegin = myChannel->getIterMyInfoBegin();
		LinkListIt iterEnd = myChannel->getIterMyInfoEnd();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin).get() == &clientInfo)
			{
				cout << "채널 count = " << (*iterBegin).use_count() << endl;
				iterBegin = myChannel->eraseClient(iterBegin); // 원래 있던 채널에서 빼기
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

