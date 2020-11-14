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
	SOCKADDR_IN servAdr, clntAdr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;

	int adrSz;
	int strLen, fdNum, i;
	char buf[BUF_SIZE];

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	FD_ZERO(&reads);

	/*
		select 함수의 두 번째 인자로 전달될 fd_set형 변수 reads에 서버 소켓을 등록하고 있다.
		이로써 데이터의 수신여부를 관찰하는 관찰대상에 서버 소켓이 포함되었다.
		참고로 클라이언트의 연결요청도 데이터의 전송을 통해서 이뤄진다.
		따라서 이후에 서버 소켓으로 수신된 데이터가 존재한다는 것은 연결요청이 있었다는 뜻으로 해석해야 한다.
	*/
	FD_SET(hServSock, &reads);

	while (1) {
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		/*
			while문으로 구성된 무한루프 내에서 select 함수가 호출되고 있다.
			select 함수의 세 번째, 그리고 네 번째 매개변수가 비어있다.
			이렇듯 관찰의 목적에 맞게 필요한 인자만 전달하면 된다.
		*/
		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
			break;

		if (fdNum == 0)
			continue;

		/*
			select 함수가 1이상 반환했을 때 실행되는 반복문이다.
			1이상 반환되었으므로, 아래의 FD_ISSET 함수를 호출하면서 상태변화가 있었던(수신된 데이터가 있는 소켓의) 파일 디스크립터를 찾고 있다.
		*/
		for (i = 0; i < reads.fd_count; i++) {
			if (FD_ISSET(reads.fd_array[i], &cpyReads)) {

				/*
					수신된 데이터가 hServSock(서버 소켓)에 있다면,
					연결 요청이기 때문에 이에 따른 처리를 진행한다.
				*/
				if (reads.fd_array[i] == hServSock) {		// connection request!
					adrSz = sizeof(clntAdr);
					hClntSock =
						accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					FD_SET(hClntSock, &reads);
					printf("connected client: %d \n", hClntSock);
				}
				/*
					수신된 데이터가 클라이언트와 연결된 소켓에 있다면,
					이에 따른 에코 서비스 처리를 진행한다.
				*/
				else {		// read message!
					strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);

					if (strLen == 0) {		// close request!
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						printf("closed client: %d \n", cpyReads.fd_array[i]);
					}
					else {
						send(reads.fd_array[i], buf, strLen, 0);    // echo!
					}
				}
			}
		}
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}