#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#define BUF_SIZE 1024

void ErrorHandling(char *message);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE];
	int strLen, i;

	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	clntAdrSize = sizeof(clntAdr);

	/*
		총 5개의 클라이언트에게 서비스를 제공하기 위한 반복문이다.
		결과적으로 accept 함수가 총 5회 호출되어 총 5개의 클라이언트에게 순서대로 에코 서비스를 제공한다.
		클라이언트와 연결이 되면, 이 에코 입출력 서비스를 소켓 close 전까지 돌린다.
	*/
	for (i = 0; i < 5; i++) {
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
		if (hClntSock == -1)
			ErrorHandling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		/*
			실제 에코 서비스가 이뤄지는 부분이다.
			읽어 들인 문자열을 그대로 전송하고 있다.
		*/
		while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
			send(hClntSock, message, strLen, 0);

		/*
			소켓을 대상으로 close 함수를 호출하고 있다.
			close 함수가 호출되면, 연결되어있던 상대방 소켓에게 EOF가 전달된다.
			즉, 클라이언트 소켓이 close 함수를 호출하면 위 while문의 조건은 false가 되어 이 함수가 실행된다.
		*/
		closesocket(hClntSock);
	}

	/*
		총 5개의 클라이언트에게 서비스를 제공하고 나면,
		마지막으로 서버 소켓을 종료하면서 프로그램을 종료한다.
	*/
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}