#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>     // for struct ip_mreq
#pragma comment(lib, "ws2_32.lib")
#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hRecvSock;
	SOCKADDR_IN adr;
	struct ip_mreq joinAdr;
	char buf[BUF_SIZE];
	int strLen;


	if (argc != 3) {
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = htonl(INADDR_ANY);
	adr.sin_port = htons(atoi(argv[2]));

	if (bind(hRecvSock, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	/*
		구조체 ip_mreq형 변수를 초기화하고 있다.
		멀티캐스트 그룹의 IP 주소와 그룹에 가입할 호스트의 IP 주소를 순서대로 초기화한다.
	*/
	joinAdr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	joinAdr.imr_interface.s_addr = htonl(INADDR_ANY);

	/*
		소켓 옵션 IP_ADD_MEMBERSHIP을 이용해서 멀티캐스트 그룹에 가입하고 있다.
		이제 위에서 지정한 멀티캐스트 그룹으로 전송되는 데이터를 수신할 준비는 모두 끝났다.
	*/
	if (setsockopt(hRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(void*)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR)
		ErrorHandling("setsock() error");

	while (1) {
		/*
			recvfrom 함수 호출을 통해서 멀티캐스트 데이터를 수신하고 있다.
			참고로 데이터를 전송한 호스트의 주소 정보를 알 필요가 없다면,
			이렇듯 recvfrom 함수의 다섯 번째와 여섯 번째 인자로 각각 NULL과 0을 전달하면 된다.
		*/
		strLen = recvfrom(hRecvSock, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (strLen < 0)
			break;
		buf[strLen] = 0;
		fputs(buf, stdout);
	}
	closesocket(hRecvSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}