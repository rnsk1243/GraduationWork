#include "ChannelHandler.h"
#include"ErrorHandler.h"

CChannelHandler::CChannelHandler()
{
}


CChannelHandler::~CChannelHandler()
{
}

bool CChannelHandler::moveNextChannel(shared_ptr<CLink> shared_clientInfo, CChannelManager& channelManager, int targetChannelNo)
{
	CLink* clientInfo = nullptr;
	if (0 < shared_clientInfo.use_count())
	{
		clientInfo = shared_clientInfo.get();
	}
	else
	{
		CErrorHandler::ErrorHandler(ERROR_SHARED_COUNT_ZORO);
		return false;
	}
	// channel����Ʈ iterator
	ChannelListIt iterBegin = channelManager.getIterChannelBegin();
	ChannelListIt iterEnd = channelManager.getIterChannelEnd();
	
	// �ű���� �ϴ� ��ȣ�� Channel ������ ���
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->getChannelNum())
		{
			cout << targetChannelNo << "�� ä�η� �̵� �մϴ�." << endl;
			(*iterBegin)->pushClient(shared_clientInfo); // ä�ο� �־��ֱ�
			clientInfo->setMyChannelNum(targetChannelNo);
			return true; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	cout << "ä�� �ű�� ����" << endl;
	return false;
}

bool CChannelHandler::exitChannel(CLink& clientInfo, CChannelManager& channelManager)
{
	CChannel* myChannel = channelManager.getMyChannel(clientInfo.getMyChannelNum());
	if (myChannel != nullptr)
	{
		cout << myChannel->getChannelNum() << "�� ä���� �����ϴ�." << endl;
		LinkListIt iterBegin = myChannel->getIterMyInfoBegin();
		LinkListIt iterEnd = myChannel->getIterMyInfoEnd();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin).get() == &clientInfo)
			{
				cout << "ä�� count = " << (*iterBegin).use_count() << endl;
				iterBegin = myChannel->eraseClient(iterBegin); // ���� �ִ� ä�ο��� ����
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

