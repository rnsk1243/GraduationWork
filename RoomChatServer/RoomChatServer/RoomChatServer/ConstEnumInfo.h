#pragma once


// 값
const int Port = 9000;
const int BufSize = 1024;
const int NameSize = 30;
const int IdPwSize = 256;
const int IntSize = 4;
const int MakeThreadNum = 3;
const int EnterChannelNum = 1; // 처음 접속 채널
const int EnterRoomPeopleLimit = 2;
const int MaxChannelNum = 5;
const int ChannelAmount = 5;
const int NoneRoom = -1;
// 취소
const int Cancel = 4444;

// recv관련 10~19 return
const int SUCCES_RECV = 10;
// recv 에러
const int ERROR_RECV = 11;

// send관련 20~29 return
const int SUCCES_SEND = 20;
// send 에러
const int ERROR_SEND = 21;

///////////// 기타 관련 70~89 return////////////
// 클라이언트 묻기 성공/
const int SUCCES_ASKCLIENT = 70;
// 클라이언트 통보 성공
const int SUCCES_NOTIFICATION = 72;
// 로그인 성공
const int SUCCES_LOGIN = 74;
// 로그인 실패
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
// 예외처리가 안된 오류
const int ERROR_EXCEPTION = 9876;

