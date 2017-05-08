#include "ChannelHandler.h"


CChannelHandler::CChannelHandler()
{
}


CChannelHandler::~CChannelHandler()
{
}

bool CChannelHandler::enterChannel(CLink* clientInfo, CChannelManager& channelManager, int targetChannelNo)
{
	// channel����Ʈ iterator
	ChannelListIt iterBegin = channelManager.getIterChannelBegin();
	ChannelListIt iterEnd = channelManager.getIterChannelEnd();
	
	// �ű���� �ϴ� ��ȣ�� Channel ������ ���
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->getChannelNum())
		{
			cout << targetChannelNo << "�� ä�η� �̵� �մϴ�." << endl;
			shared_ptr<CLink> pushObj(clientInfo); // �޸� ������ü���� �ѱ��
			(*iterBegin)->pushClient(pushObj); // ä�ο� �־��ֱ�
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

