#pragma once

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#define WM_SOCKET WM_USER

extern HWND hWnd;
SOCKET client_socket;

bool startup_client()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("124.63.153.55");
	server_addr.sin_port = htons(8000);
	if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
		return 0;

	WSAAsyncSelect(client_socket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	return 1;
}

void cleanup_client()
{
	closesocket(client_socket);
	WSACleanup();
}
