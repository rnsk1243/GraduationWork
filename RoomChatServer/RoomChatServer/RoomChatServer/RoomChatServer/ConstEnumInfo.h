#pragma once
#include<string>
#include<iostream>
using namespace std;

const string StartCardInventory("01/00|02/00|03/00|04/00|05/00|06/00|07/00|08/00|09/00|10/00|11/00"); // 카드번호/카드갯수
const string NameMemberInfoTxt = "MemberInfo.txt";
const string NameMemberCardInfoTxt = "MemberCardInfo.txt";
const string CardEmpty = "00";
// 값
const int Port = 9000;
const int BufSize = 1024;
const int CardNameBuf = 64;
const int ExcelBufSize = 4096;
const int NameSize = 30;
const int IdPwSize = 256;
const int IntSize = 4;
const int MakeThreadNum = 3;
const int EnterChannelNum = 1; // 처음 접속 채널
const int EnterRoomPeopleLimit = 2;
const int MaxChannelNum = 5;
const int ChannelAmount = 5;
const int NoneRoom = -1;
const int CardNumCols = 0; // 액셀파일 카드 번호 열번호
const int CardNameCols = 1; 
const int CardProbCols = 2;
const int CardStatCols = 3;
const int CardCost = 20;
const int StartMoney = 100; // 시작 머니
const int ErrorCardNum = 999;


