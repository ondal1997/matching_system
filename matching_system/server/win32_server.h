#pragma once

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#define WM_ACCEPT WM_USER
#define WM_SOCKET WM_USER+1

extern HWND hWnd;
SOCKET server_socket;

void close_client(SOCKET client_socket, bool force = false)
{
	if (force)
	{
		LINGER linger = { 1, 0 };
		setsockopt(client_socket, SOL_SOCKET, SO_LINGER, (const char*)&linger, sizeof(linger));
	}

	closesocket(client_socket);
}

bool startup_server()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(8000);
	if (bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
		return 0;

	if (listen(server_socket, 32) == SOCKET_ERROR)
		return 0;

	WSAAsyncSelect(server_socket, hWnd, WM_ACCEPT, FD_ACCEPT);
	return 1;
}

void cleanup_server()
{
	closesocket(server_socket);
	WSACleanup();
}
