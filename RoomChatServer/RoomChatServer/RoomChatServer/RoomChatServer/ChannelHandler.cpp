#include "ChannelHandler.h"

CChannelHandler::CChannelHandler()
{
}


CChannelHandler::~CChannelHandler()
{
}

bool CChannelHandler::MoveNextChannel(const shared_ptr<CLink>& shared_clientInfo, CChannelManager& channelManager, int targetChannelNo)
{
	CLink* clientInfo = nullptr;
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
	}
	else
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
		return false;
	}
	// channel리스트 iterator
	ChannelVecIt iterBegin = channelManager.GetIterChannelBegin();
	ChannelVecIt iterEnd = channelManager.GetIterChannelEnd();
	
	// 옮기고자 하는 번호의 Channel 포인터 얻기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->GetChannelNum())
		{
			cout << targetChannelNo << "번 채널로 이동 합니다." << endl;
			(*iterBegin)->PushClient(shared_clientInfo); // 채널에 넣어주기
			clientInfo->SetMyChannelNum(targetChannelNo);
			return true; // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}
	cout << "채널 옮기기 실패" << endl;
	return false;
}

bool CChannelHandler::ExitChannel(CLink& clientInfo, CChannelManager& channelManager)
{
	CChannel* myChannel = channelManager.GetMyChannel(clientInfo.GetMyChannelNum());
	if (myChannel != nullptr)
	{
		cout << myChannel->GetChannelNum() << "번 채널을 나갑니다." << endl;
		LinkListIt iterBegin = myChannel->GetIterMyInfoBegin();
		LinkListIt iterEnd = myChannel->GetIterMyInfoEnd();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin).get() == &clientInfo)
			{
				cout << "채널 count = " << (*iterBegin).use_count() << endl;
				iterBegin = myChannel->EraseClient(iterBegin); // 원래 있던 채널에서 빼기
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

