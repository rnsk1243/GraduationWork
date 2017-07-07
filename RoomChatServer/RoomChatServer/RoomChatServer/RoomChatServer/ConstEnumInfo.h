#pragma once
#include<string>
#include<iostream>
using namespace std;

enum WhatCardCurserSize
{
	CardNum, CardAmount, CardExp, CardEvolution, CardStar
};

const string CommandEnter = "e"; // �� ����
const string CommandChannal = "c"; // ä�� ����
const string CommandMakeRoom = "m"; // �� �����
const string CommandOutRoom = "o"; // �� ������
const string CommandMergeRoom = "i"; // �� ����
const string CommandChangeName = "n"; // �̸� ����
const string CommandGachar = "Gachar"; // ī�� �̱�
const string CommandCompose = "Compose"; // ī�� �ռ�
const string CommandEvolution = "Evolution"; // ī�� ��ȭ

const string StartCardInventory("01/00/00/0/0|02/00/00/0/0|03/00/00/0/0|04/00/00/0/0|05/00/00/0/0|06/00/00/0/0|07/00/00/0/0|08/00/00/0/0|09/00/00/0/0|10/00/00/0/0|11/00/00/0/0"); // ī���ȣ/ī�尹��
const int NameMemberCardInfoTxtCapacity = 9;
const string NameMemberInfoTxt = "MemberInfo.txt";
const string NameMemberCardInfoTxt = "MemberCardInfo.txt";
const string MakeNextJoinNumberTxt = "MakeNextJoinNumber.txt";
const string CardEmpty = "00";
// ��
const int Port = 9000;
const int BufSize = 1024;
const int CardNameBuf = 64;
const int ExcelBufSize = 4096;
const int NameSize = 30;
const int IdPwSize = 256;
const int IntSize = 4;
const int MakeThreadNum = 3;
const int EnterChannelNum = 1; // ó�� ���� ä��
const int EnterRoomPeopleLimit = 2;
const int MaxChannelNum = 5;
const int ChannelAmount = 5;
const int NoneRoom = -1;
const int CardNumCols = 0; // �׼����� ī�� ��ȣ ����ȣ
const int CardNameCols = 1; 
const int CardProbCols = 2;
const int CardStatCols = 3;
const int CardExpCols = 4;
const int CardCost = 20;
const int StartMoney = 100; // ���� �Ӵ�
const int ErrorCardNum = 999;
const int IndexUserPK = 0; // .txt���Ͽ��� �� ������ 0 | ��Ÿ�������� 1�� ����
const int IndexUserID = 1; 
const int IndexUserPW = 2;
const int CardKind = 11;
const int CardInfoSize = 13;
const int CardTemplateSize = (CardKind * CardInfoSize) + 3; // (1 + 2) // pk��ȣ �� |(1) �� �ٹٲ� '\n'(2) �ؼ� 3�� ����
const int IndexCardInfoTxtPK = 0;
const int IndexCardInfoTxtNumber = 0;
const int IndexCardInfoTxtAmount = 1;
const int IndexCardInfoTxtExp = 2;
const int IndexCardInfoTxtEvolution = 3;
const int IndexCardInfoTxtStar = 4;
const int SaveCardAmountCipherTextSize = 2;// txt�� ����Ǵ� �ڸ���, �� �ڸ�, �� �ڸ� ...
const int SaveCardExpCipherTextSize = 2;
const int SaveCardEvolutionCipherTextSize = 1;
const int SaveCardStarCipherTextSize = 1;
