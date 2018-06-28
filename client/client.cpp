#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")

#include <WS2tcpip.h>

#include <winsock2.h> //клиент
#include <iostream>
using namespace std;

int main()
{
	SOCKET m_socket;
	addrinfo *m_addrinfo;

	WSADATA wsaData;
	
	const char *ip = "127.0.0.1"; //"192.168.12.47";
	const char *port = "5444";
	printf("Creating windows socket for %s:%s\n", ip, port);

	// Initialize library
	printf("Calling WSAStartup()\n");

	int error = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (error != 0)
	{
		printf("WSAStartup: %d\n", error);
		return 0;
	}

	// Initialize hint structure
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	printf("Calling getaddrinfo()\n");
	error = getaddrinfo(ip, port, &hints, &m_addrinfo);
	if (error != 0)
	{
		printf("Getaddrinfo: %d\n", error);
		WSACleanup();

		return 0;
	}

	// Инициализация сокета
	printf("Creating socket\n");
	m_socket = socket(m_addrinfo->ai_family, m_addrinfo->ai_socktype, m_addrinfo->ai_protocol);
	if (m_socket == INVALID_SOCKET)
	{
		printf("Re-creating socket failure: %d\n", WSAGetLastError());

		freeaddrinfo(m_addrinfo);
		WSACleanup();

		return 0;
	}

	if (connect(m_socket, m_addrinfo->ai_addr, (int)m_addrinfo->ai_addrlen) == SOCKET_ERROR)
	{
		printf("Connect error: %d\n", WSAGetLastError());

		return 0;
	}






	char str[100];
	cout << "\n input str:" << endl;
	cin >> str;

	int asa = send(m_socket, str, sizeof(str), 0);
	//int asa= recv(m_socket, str, 255, 0);
	if (asa == SOCKET_ERROR)
	{
		printf("Error occured: %d\n", WSAGetLastError());

		return 0;
	}



	// безопасное закрытие сокета с освобождением всех ресурсов
	char _buff[1024] = { 0 };
	shutdown(m_socket, SD_BOTH); // запрет отправки и приема на сокете

	while (recv(m_socket, _buff, 1024, 0) != -1);

	linger linger;                  // структура установки параметров сокета       
	linger.l_linger = 0;            // как долго сокет должен оставаться открытым
	linger.l_onoff = 1;             // разрешение использовать linger 
	setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char *)&linger, sizeof(linger));

	if (m_socket == NULL)
	{
		return 0;
	}

	if (closesocket(m_socket))
	{
		printf("Closesocket: %d\n", WSAGetLastError());
		freeaddrinfo(m_addrinfo);
		WSACleanup();

		return 0;
	}

	m_socket = NULL;


	system("pause");
	return 0;
}