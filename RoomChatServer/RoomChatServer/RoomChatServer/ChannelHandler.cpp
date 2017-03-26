#include "ChannelHandler.h"


CChannelHandler::CChannelHandler()
{
}


CChannelHandler::~CChannelHandler()
{
}

bool CChannelHandler::enterChannel(CLink* clientInfo, CRoomChannelManager* roomChannelManager, int targetChannelNo)
{
	// channel����Ʈ iterator
	ChannelListIt iterBegin = roomChannelManager->getIterChannelBegin();
	ChannelListIt iterEnd = roomChannelManager->getIterChannelEnd();
	
	// �ű���� �ϴ� ��ȣ�� Channel ������ ���
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->getChannelNum())
		{
			cout << targetChannelNo << "�� ä�η� �̵� �մϴ�." << endl;
			(*iterBegin)->pushClient(clientInfo); // ä�ο� �־��ֱ�
			clientInfo->setMyChannelNum(targetChannelNo);
			return true; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	cout << "ä�� �ű�� ����" << endl;
	return false;
}

bool CChannelHandler::exitChannel(CLink* clientInfo, CRoomChannelManager* roomChannelManager)
{
	CChannel* myChannel = roomChannelManager->getMyChannel(clientInfo->getMyChannelNum());
	cout << myChannel->getChannelNum() << "�� ä���� �����ϴ�." << endl;

	if (myChannel != nullptr)
	{
		LinkListIt iterBegin = myChannel->getIterMyInfoBegin();
		LinkListIt iterEnd = myChannel->getIterMyInfoEnd();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin) == clientInfo)
			{
				iterBegin = myChannel->eraseClient(iterBegin); // ���� �ִ� �濡�� ����
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

bool CChannelHandler::pushChannel(CChannel * newChannel)
{
	return false;
}
