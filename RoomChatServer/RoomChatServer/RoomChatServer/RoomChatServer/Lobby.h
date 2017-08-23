#pragma once
#include"Listener.h"
#include"GuestLink.h"
#include<vector>
//
//typedef shared_ptr<CGuestLink> GuestLinkPtr;
//typedef list<GuestLinkPtr> GuestList;
//typedef GuestList::iterator GuestListIt;

class CLobby
{
	int Login(CGuestLink& guest, CListener& listener, vector<string>& tempUserInfo);
	int JoinMember(CGuestLink& guest, CListener& listener, vector<string>& tempUserInfo);
	int ChooseMenu(const char * message, CGuestLink& guest);
	int NextUserNum;
	//GuestList mGuestList;
	//MUTEX mRAII_GuestMUTEX;
public:
	CLobby(int NextUserNum_):NextUserNum(NextUserNum_){}
	CLobby(const CLobby&) = delete;
	CLobby& operator=(const CLobby&) = delete;
	~CLobby(){}
	int ActionServiceLobby(CGuestLink& guest, CListener& listener, vector<string>& tempUserInfo);
	//void PushGuest(const GuestLinkPtr & newGuest);
	//void EraseGuest(const GuestLinkPtr & targetGuest);
};

