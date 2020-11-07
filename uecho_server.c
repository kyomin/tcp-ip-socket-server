#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET servSock;
	char message[BUF_SIZE];
	int strLen;
	int clntAdrSz;

	SOCKADDR_IN servAdr, clntAdr;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	/*
		UDP 소켓의 생성을 위해서 socket 함수의 두 번째 인자로 SOCK_DGRAM을 전달하고 있다.
	*/
	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("UDP socket creation error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	while (1) {
		clntAdrSz = sizeof(clntAdr);

		/*
			servAdr에 할당한 주소로 전달되는 모든 데이터를 수신하고 있다.
			물론 데이터의 전달 대상에는 제한이 없다.
		*/
		strLen = recvfrom(servSock, message, BUF_SIZE, 0,
			(SOCKADDR*)&clntAdr, &clntAdrSz);

		/*
			recvfrom 함수 호출을 통해 데이터를 전송한 이의 주소 정보를 clntAdr에 얻게 되었다.
			바로 이 주소 정보를 이용해서 수신된 데이터를 역으로 재전송하고 있다.
		*/
		sendto(servSock, message, strLen, 0,
			(SOCKADDR*)&clntAdr, sizeof(clntAdr));
	}
	closesocket(servSock);
	WSACleanup();
	
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}