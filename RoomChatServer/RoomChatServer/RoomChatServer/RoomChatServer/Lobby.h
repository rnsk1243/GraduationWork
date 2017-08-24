#pragma once
#include"Listener.h"
#include"GuestLink.h"
#include<vector>

class CLobby
{
	void AskedID(const CGuestLink * guest, string & id);
	void AskedPW(const CGuestLink * guest, string & pw);
	int Login(const CGuestLink * guest, vector<string>& tempUserInfo);
	int JoinMember(const CGuestLink* guest, vector<string>& tempUserInfo);
	int ChooseMenu(const char * message, const CGuestLink* guest);
	int mNextUserNum;
	CLobby(int NextUserNum_);
	void SetNextUserPKNumber();
public:
	static CLobby* GetInstance(const int& NextUserNum);
	CLobby(const CLobby&) = delete;
	CLobby& operator=(const CLobby&) = delete;
	~CLobby(){}
	int ActionServiceLobby(const CGuestLink* guest, vector<string>& tempUserInfo);
	//void PushGuest(const GuestLinkPtr & newGuest);
	//void EraseGuest(const GuestLinkPtr & targetGuest);
};
static CLobby* LobbyStatic = CLobby::GetInstance(0);
