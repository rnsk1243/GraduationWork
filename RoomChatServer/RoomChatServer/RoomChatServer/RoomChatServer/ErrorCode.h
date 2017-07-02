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

	///////////// 기타 관련 60~89 return////////////
	// 잘못 입력
	ERROR_WRONG_INPUT = 69,
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
	ERROR_DELETE_SOCKET = 99,
	ERROR_SHARED_COUNT_ZORO = 101,
	ERROR_MONEY_FAIL = 103,
	SUCCES_COMMAND_MESSAGE = 104,
	ERROR_GACHAR = 105,
	SUCCES_GACHAR = 106,
	ERROR_NEWCARD = 107, // 서버 .txt 파일에 저장 할 준비가 되어있지 않음
	ERROR_SAVE_CARD = 109, // 유저 보유 카드 저장 에러
	ERROR_SAVE_EXP = 111,
	ERROR_SAVE_EVOLUTION = 113,
	ERROR_SAVE_STAR = 115,
	ERROR_INIT_USER_CARD = 117, // 유저 로그인시 보유 카드정보 불러오기 실패
	ERROR_CARDEXP_FULL = 119, // 카드 경험치 가득이라 경험치 더 얻을 수 없음.
	ERROR_CURSER_SIZE = 121, // 커서 이동 크기 잘 못 가져옴
	ERROR_COMPOSE_NULL_CARD = 123, // 합성할 카드가 없다.
	ERROR_COMPOSE_EVOUTION_CARD = 125, // 진화해야할 카드
	ERROR_COMPOSE_NO_EVOUTION_CARD = 127, // 아직 진화불가(합성해야함.)
	ERROR_INCREACE_CARD = 129,
	ERROR_DECREACE_CARD = 131,
	ERROR_EVOLUTION_CARD = 133,
	ERROR_NULL_CARD_ITERATOR = 135, // 카드를 찾지 못해서 반복자를 가져오지 못함.
	//////////////////////////////////////////////
	// 예외처리가 안된 오류
	ERROR_EXCEPTION = 9876
};

enum EnumInfoCode
{
	INFO_NEW_EVOLUTION = 1
};