#pragma once
#include<vector>
#include<iostream>
#include"Channel.h"

using namespace std;
class CChannel;
class CLink;
class CErrorHandler;


typedef shared_ptr<CChannel> ChannelPtr;
typedef vector<ChannelPtr> ChannelVector;
typedef ChannelVector::iterator ChannelVecIt;

class CChannelManager
{
	ChannelVector mChannels;
	MUTEX mRAII_ChannelManagerMUTEX;
	//CRITICAL_SECTION CS_Channel;
	void PushChannel(const ChannelPtr& shared_newChannel);
	CChannel * GetMyChannel(int ChannelNum);
public:
	CChannelManager();
	~CChannelManager();
	CChannelManager& operator=(const CChannelManager&) = delete;
	CChannelManager(const CChannelManager&) = delete;
	// 채널옮김 함수(옮기는 링크, 옮기고싶은 채널 번호)
	bool MoveChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber);
	// 채널 에서 나가기 (나가고자하는 소켓)
	bool ExitChannel(const LinkPtr& shared_clientInfo);
	// 원래 있던 채널로 돌아가기
	bool EnterMyChannel(const LinkPtr& shared_clientInfo);
	// 해당 링크가 들어있는 수다
	void Talk(const LinkPtr& shared_clientInfo, const string& message, int flags = 0);
};

