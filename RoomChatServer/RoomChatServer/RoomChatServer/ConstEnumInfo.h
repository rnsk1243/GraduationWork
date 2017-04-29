#pragma once


// ��
const int Port = 9000;
const int BufSize = 1024;
const int NameSize = 30;
const int IdPwSize = 256;
const int IntSize = 4;
const int MakeThreadNum = 3;
const int EnterChannelNum = 1; // ó�� ���� ä��
const int EnterRoomPeopleLimit = 2;
const int MaxChannelNum = 5;
const int ChannelAmount = 5;
const int NoneRoom = -1;
// ���
const int Cancel = 4444;

// recv���� 10~19 return
const int SUCCES_RECV = 10;
// recv ����
const int ERROR_RECV = 11;

// send���� 20~29 return
const int SUCCES_SEND = 20;
// send ����
const int ERROR_SEND = 21;

///////////// ��Ÿ ���� 70~89 return////////////
// Ŭ���̾�Ʈ ���� ����/
const int SUCCES_ASKCLIENT = 70;
// Ŭ���̾�Ʈ �뺸 ����
const int SUCCES_NOTIFICATION = 72;
// �α��� ����
const int SUCCES_LOGIN = 74;
// �α��� ����
const int ERROR_LOGIN = 75;
const int OVERLAPID = 76;
const int SUCCES_JOIN = 78;
const int ERROR_JOIN = 79;
const int SUCCES_MENUOUT = 80;
const int ERROR_MENUOUT = 81;
const int ERROR_GET_CHANNEL = 83;
const int ERROR_GET_ROOM = 85;
const int ERROR_ENTER_CHANNEL = 87;
const int SUCCES_COMMAND = 88;
const int ERROR_COMMAND = 89;
const int ERROR_EXIT_CHANNEL = 91;
const int ERROR_EXIT_ROOM = 93;
const int ERROR_MAKE_ROOM = 95;
const int ERROR_ENTER_ROOM = 97;
//////////////////////////////////////////////
// ����ó���� �ȵ� ����
const int ERROR_EXCEPTION = 9876;

