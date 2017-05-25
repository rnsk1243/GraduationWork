#pragma once

enum EnumErrorCode
{
	// ���
	Cancel = 4444,

	// recv���� 10~19 return
	SUCCES_RECV = 10,
	// recv ����
	ERROR_RECV = 11,
	SUCCES_RECV_EVERY_SEND = 12, // recv������� ��� Ŭ���̾�Ʈ���� �˷����� ���

	// enum���� ������

	// send���� 20~29 return
	SUCCES_SEND = 20,
	// send ����
	ERROR_SEND = 21,

	///////////// ��Ÿ ���� 60~89 return////////////
	// �߸� �Է�
	ERROR_WRONG_INPUT = 69,
	// Ŭ���̾�Ʈ ���� ����/
	SUCCES_ASKCLIENT = 70,
	// Ŭ���̾�Ʈ �뺸 ����
	SUCCES_NOTIFICATION = 72,
	// �α��� ����
	SUCCES_LOGIN = 74,
	// �α��� ����
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
	ERROR_SERIALIZE_TO_ARRAY = 103,
	ERROR_MESSAGE_NPTR = 105,
	SUCCES_MULTI_SENDN = 106,
	ERROR_RECV_ZERO = 107,
	ERROR_PARSE = 109,
	ERROR_LINKINFO_NULL = 111,
	SUCCES_SINGLE_SENDN = 112,
	//////////////////////////////////////////////
	// ����ó���� �ȵ� ����
	ERROR_EXCEPTION = 9876
};

