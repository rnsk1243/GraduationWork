#include "ChannelHandlere.h"



CChannelHandlere::CChannelHandlere()
{
}


CChannelHandlere::~CChannelHandlere()
{
}

bool CChannelHandlere::enterChannel(CMyInfo* clientInfo, CLink* link, int targetChannelNo)
{
	// channel����Ʈ iterator
	ChannelListIt iterBegin = link->getIterChannelBegin();
	ChannelListIt iterEnd = link->getIterChannelEnd();
	// �ű���� �ϴ� ��ȣ�� Channel ������ ���
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->getChannelNum())
		{
			cout << targetChannelNo << "�� ä�η� �̵� �մϴ�." << endl;
			(*iterBegin)->pushSocket(clientInfo->getClientSocket()); // ä�ο� �־��ֱ�
			clientInfo->setMyChannelNum(targetChannelNo);
			return true; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	cout << "ä�� �ű�� ����" << endl;
	return false;
}

bool CChannelHandlere::exitChannel(CMyInfo* clientInfo, CLink* link)
{
	CChannel* myChannel = link->getMyChannel(clientInfo->getMyChannelNum());
	cout << myChannel->getChannelNum() << "�� ä���� �����ϴ�." << endl;

	if (myChannel != nullptr)
	{
		SocketListIt iterBegin = myChannel->getIterBegin();
		SocketListIt iterEnd = myChannel->getIterEnd();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin) == clientInfo->getClientSocket())
			{
				iterBegin = myChannel->eraseSocket(iterBegin); // ���� �ִ� �濡�� ����
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

bool CChannelHandlere::pushChannel(CChannel * newChannel)
{
	return false;
}
