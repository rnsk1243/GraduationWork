#pragma once

enum EnumErrorCode
{
	// 취소
	Cancel = 4444,

	// recv관련 10~19 return
	SUCCES_RECV = 10,
	// recv 에러
	ERROR_RECV = 11,

	// enum으로 만들어라

	// send관련 20~29 return
	SUCCES_SEND = 20,
	// send 에러
	ERROR_SEND = 21,

	///////////// 기타 관련 70~89 return////////////
	// 클라이언트 묻기 성공/
	SUCCES_ASKCLIENT = 70,
	// 클라이언트 통보 성공
	SUCCES_NOTIFICATION = 72,
	// 로그인 성공
	SUCCES_LOGIN = 74,
	// 로그인 실패
	ERROR_LOGIN = 75,
	OVERLAPID = 76,
	SUCCES_JOIN = 78,
	ERROR_JOIN = 79,
	SUCCES_MENUOUT = 80,
	ERROR_MENUOUT = 81,
	ERROR_GET_CHANNEL = 83,
	ERROR_GET_ROOM = 85,
	ERROR_ENTER_CHANNEL = 87,
	SUCCES_COMMAND = 88,
	ERROR_COMMAND = 89,
	ERROR_EXIT_CHANNEL = 91,
	ERROR_EXIT_ROOM = 93,
	ERROR_MAKE_ROOM = 95,
	ERROR_ENTER_ROOM = 97,
	//////////////////////////////////////////////
	// 예외처리가 안된 오류
	ERROR_EXCEPTION = 9876
};

