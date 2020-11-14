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
		select �Լ��� �� ��° ���ڷ� ���޵� fd_set�� ���� reads�� ���� ������ ����ϰ� �ִ�.
		�̷ν� �������� ���ſ��θ� �����ϴ� ������� ���� ������ ���ԵǾ���.
		����� Ŭ���̾�Ʈ�� �����û�� �������� ������ ���ؼ� �̷�����.
		���� ���Ŀ� ���� �������� ���ŵ� �����Ͱ� �����Ѵٴ� ���� �����û�� �־��ٴ� ������ �ؼ��ؾ� �Ѵ�.
	*/
	FD_SET(hServSock, &reads);

	while (1) {
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		/*
			while������ ������ ���ѷ��� ������ select �Լ��� ȣ��ǰ� �ִ�.
			select �Լ��� �� ��°, �׸��� �� ��° �Ű������� ����ִ�.
			�̷��� ������ ������ �°� �ʿ��� ���ڸ� �����ϸ� �ȴ�.
		*/
		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
			break;

		if (fdNum == 0)
			continue;

		/*
			select �Լ��� 1�̻� ��ȯ���� �� ����Ǵ� �ݺ����̴�.
			1�̻� ��ȯ�Ǿ����Ƿ�, �Ʒ��� FD_ISSET �Լ��� ȣ���ϸ鼭 ���º�ȭ�� �־���(���ŵ� �����Ͱ� �ִ� ������) ���� ��ũ���͸� ã�� �ִ�.
		*/
		for (i = 0; i < reads.fd_count; i++) {
			if (FD_ISSET(reads.fd_array[i], &cpyReads)) {

				/*
					���ŵ� �����Ͱ� hServSock(���� ����)�� �ִٸ�,
					���� ��û�̱� ������ �̿� ���� ó���� �����Ѵ�.
				*/
				if (reads.fd_array[i] == hServSock) {		// connection request!
					adrSz = sizeof(clntAdr);
					hClntSock =
						accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					FD_SET(hClntSock, &reads);
					printf("connected client: %d \n", hClntSock);
				}
				/*
					���ŵ� �����Ͱ� Ŭ���̾�Ʈ�� ����� ���Ͽ� �ִٸ�,
					�̿� ���� ���� ���� ó���� �����Ѵ�.
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