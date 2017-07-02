#pragma once

enum EnumErrorCode
{
	// ���
	Cancel = 4444,

	// recv���� 10~19 return
	SUCCES_RECV = 10,
	// recv ����
	ERROR_RECV = 11,

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
	ERROR_MONEY_FAIL = 103,
	SUCCES_COMMAND_MESSAGE = 104,
	ERROR_GACHAR = 105,
	SUCCES_GACHAR = 106,
	ERROR_NEWCARD = 107, // ���� .txt ���Ͽ� ���� �� �غ� �Ǿ����� ����
	ERROR_SAVE_CARD = 109, // ���� ���� ī�� ���� ����
	ERROR_SAVE_EXP = 111,
	ERROR_SAVE_EVOLUTION = 113,
	ERROR_SAVE_STAR = 115,
	ERROR_INIT_USER_CARD = 117, // ���� �α��ν� ���� ī������ �ҷ����� ����
	ERROR_CARDEXP_FULL = 119, // ī�� ����ġ �����̶� ����ġ �� ���� �� ����.
	ERROR_CURSER_SIZE = 121, // Ŀ�� �̵� ũ�� �� �� ������
	ERROR_COMPOSE_NULL_CARD = 123, // �ռ��� ī�尡 ����.
	ERROR_COMPOSE_EVOUTION_CARD = 125, // ��ȭ�ؾ��� ī��
	ERROR_COMPOSE_NO_EVOUTION_CARD = 127, // ���� ��ȭ�Ұ�(�ռ��ؾ���.)
	ERROR_INCREACE_CARD = 129,
	ERROR_DECREACE_CARD = 131,
	ERROR_EVOLUTION_CARD = 133,
	ERROR_NULL_CARD_ITERATOR = 135, // ī�带 ã�� ���ؼ� �ݺ��ڸ� �������� ����.
	//////////////////////////////////////////////
	// ����ó���� �ȵ� ����
	ERROR_EXCEPTION = 9876
};

enum EnumInfoCode
{
	INFO_NEW_EVOLUTION = 1
};