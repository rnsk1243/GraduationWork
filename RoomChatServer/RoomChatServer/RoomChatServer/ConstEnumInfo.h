#pragma once

// 값
const int Port = 9000;
const int BufSize = 1024;
const int NameSize = 30;
const int IdPwSize = 256;
const int IntSize = 4;
const int MakeThreadNum = 3;
const int EnterChannelNum = 1; // 처음 접속 채널
const int EnterRoomPeopleLimit = 4;
const int MaxChannelNum = 5;
const int ChannelAmount = 5;
const int NoneRoom = -1;
const int RecvSizeByteSize = 6;
const int RoomNameSize = 256;
const int ClientNumNone = 4; // 메세지를 보낸 클라이언트에게 보낼때 g_DataSize.clientNum 프로퍼티 값

//const int NullNameError = -1;
//const int BufSize = 1024;
//const int NameSize = 30;
//const int IdPwSize = 256;
//const int IntSize = 4;
////const int G_MessageSize = 32;
////const int G_TransformSize = 48;
////const int G_VectorSize = 32;
//const int OccuredError = -1;
//const int SerializeFail = -3;
//const int ParseFail = -4;
//const int SuccesRecv = 1;
//const int SuccesCommand = 2;
//const int SuccesSend = 3;
//const int EnterRoomPeopleLimit = 2;
//const int MaxChannelNum = 5;
//const int EnterChannelNum = 1; // 처음 접속 채널
//const int ChannelAmount = 5;
//const int Port = 9000;
//const int NoneRoom = -1;
//const int MakeThreadNum = 3;
//const int ERROR_RECV_ZERO = 41;