#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")


int _tmain(int argc, _TCHAR* argv[])
{
	// 윈속 초기화
	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		puts("ERROR: 윈속을 초기화 할 수 없습니다,");
		return 0;
	}

	// 1. 접속대기 소켓 생성
	SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		puts("ERROR: 소켓을 생성할 수 없습니다,");
		return 0;
	}

	// 2. 포트 바인딩 및 연결
	SOCKADDR_IN svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(25000);
	svraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (::connect(hSocket,
		(SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		puts("ERROR: 서버에 연결할 수 없습니다.");
		return 0;
	}

	// 3. 채팅 메시지 송/수신
	char szBuffer[128] = { 0 };
	while (1)
	{
		// 사용자로부터 문자열 입력 받음
		gets_s(szBuffer);
		if (strcmp(szBuffer, "EXIT") == 0)	break;

		// 사용자가 입력한 문자열을 서버에 전송함
		::send(hSocket, szBuffer, strlen(szBuffer) + 1, 0);

		// 서버로부터 방금 보낸 문자열에 대한 에코 메시지를 수신함
		memset(szBuffer, 0, sizeof(szBuffer));
		::recv(hSocket, szBuffer, sizeof(szBuffer), 0);
		printf("From server: %s\n", szBuffer);
	}

	// 소켓 닫고 종료
	//::shutdown(hSocket, SD_BOTH);
	::closesocket(hSocket);

	// 윈속 해제
	::WSACleanup();
	return 0;
}