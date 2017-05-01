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

	///////////// ��Ÿ ���� 70~89 return////////////
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
	//////////////////////////////////////////////
	// ����ó���� �ȵ� ����
	ERROR_EXCEPTION = 9876
};

