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
	// channel����Ʈ iterator
	ChannelVecIt iterBegin = channelManager.GetIterChannelBegin();
	ChannelVecIt iterEnd = channelManager.GetIterChannelEnd();
	
	// �ű���� �ϴ� ��ȣ�� Channel ������ ���
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->GetChannelNum())
		{
			cout << targetChannelNo << "�� ä�η� �̵� �մϴ�." << endl;
			(*iterBegin)->PushClient(shared_clientInfo); // ä�ο� �־��ֱ�
			clientInfo->SetMyChannelNum(targetChannelNo);
			return true; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	cout << "ä�� �ű�� ����" << endl;
	return false;
}

bool CChannelHandler::ExitChannel(CLink& clientInfo, CChannelManager& channelManager)
{
	CChannel* myChannel = channelManager.GetMyChannel(clientInfo.GetMyChannelNum());
	if (myChannel != nullptr)
	{
		cout << myChannel->GetChannelNum() << "�� ä���� �����ϴ�." << endl;
		LinkListIt iterBegin = myChannel->GetIterMyInfoBegin();
		LinkListIt iterEnd = myChannel->GetIterMyInfoEnd();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin).get() == &clientInfo)
			{
				cout << "ä�� count = " << (*iterBegin).use_count() << endl;
				iterBegin = myChannel->EraseClient(iterBegin); // ���� �ִ� ä�ο��� ����
				break;
			}
		}
		return true;
	}
	else
	{
		cout << "ä�� ������ ����" << endl;
		return false;
	}
}

