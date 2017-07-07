#pragma once
#include<string>
#include<iostream>
using namespace std;

enum WhatCardCurserSize
{
	CardNum, CardAmount, CardExp, CardEvolution, CardStar
};

const string CommandEnter = "e"; // 방 입장
const string CommandChannal = "c"; // 채널 변경
const string CommandMakeRoom = "m"; // 방 만들기
const string CommandOutRoom = "o"; // 방 나가기
const string CommandMergeRoom = "i"; // 방 병합
const string CommandChangeName = "n"; // 이름 변경
const string CommandGachar = "Gachar"; // 카드 뽑기
const string CommandCompose = "Compose"; // 카드 합성
const string CommandEvolution = "Evolution"; // 카드 진화

const string StartCardInventory("01/00/00/0/0|02/00/00/0/0|03/00/00/0/0|04/00/00/0/0|05/00/00/0/0|06/00/00/0/0|07/00/00/0/0|08/00/00/0/0|09/00/00/0/0|10/00/00/0/0|11/00/00/0/0"); // 카드번호/카드갯수
const int NameMemberCardInfoTxtCapacity = 9;
const string NameMemberInfoTxt = "MemberInfo.txt";
const string NameMemberCardInfoTxt = "MemberCardInfo.txt";
const string MakeNextJoinNumberTxt = "MakeNextJoinNumber.txt";
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
const int CardExpCols = 4;
const int CardCost = 20;
const int StartMoney = 100; // 시작 머니
const int ErrorCardNum = 999;
const int IndexUserPK = 0; // .txt파일에서 맨 왼쪽을 0 | 나타날때마나 1씩 증가
const int IndexUserID = 1; 
const int IndexUserPW = 2;
const int CardKind = 11;
const int CardInfoSize = 13;
const int CardTemplateSize = (CardKind * CardInfoSize) + 3; // (1 + 2) // pk번호 옆 |(1) 랑 줄바꿈 '\n'(2) 해서 3을 더함
const int IndexCardInfoTxtPK = 0;
const int IndexCardInfoTxtNumber = 0;
const int IndexCardInfoTxtAmount = 1;
const int IndexCardInfoTxtExp = 2;
const int IndexCardInfoTxtEvolution = 3;
const int IndexCardInfoTxtStar = 4;
const int SaveCardAmountCipherTextSize = 2;// txt에 저장되는 자릿수, 한 자리, 두 자리 ...
const int SaveCardExpCipherTextSize = 2;
const int SaveCardEvolutionCipherTextSize = 1;
const int SaveCardStarCipherTextSize = 1;
