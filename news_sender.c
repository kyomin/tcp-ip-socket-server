#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>     // for IP_MULTICAST_TTL option
#pragma comment(lib, "ws2_32.lib")
#define TTL 64
#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hSendSock;
	SOCKADDR_IN mulAdr;
	int timeLive = TTL;
	FILE *fp;
	char buf[BUF_SIZE];

	if (argc != 3) {
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	/*
		멀티캐스트 데이터의 송수신은 UDP 소켓을 대상으로 하기 때문에,
		이 문장에서는 UDP 소켓을 생성하고 있다.
	*/
	hSendSock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&mulAdr, 0, sizeof(mulAdr));

	/*
		데이터를 전송할 주소 정보를 설정하고 있다.
		중요한 것은 반드시 IP 주소를 멀티캐스트 주소로 설정해야 한다는 사실이다.
	*/
	mulAdr.sin_family = AF_INET;
	mulAdr.sin_addr.s_addr = inet_addr(argv[1]);
	mulAdr.sin_port = htons(atoi(argv[2]));

	/*
		소켓의 TTL 정보를 지정하고 있다.
		Sender(송신자)에서는 반드시 해야 할 일이다.
	*/
	setsockopt(hSendSock, IPPROTO_IP,
		IP_MULTICAST_TTL, (void*)&timeLive, sizeof(timeLive));

	if ((fp = fopen("news.txt", "r")) == NULL)
		ErrorHandling("fopen() error");

	/*
		실제 데이터 전송이 이뤄지는 영역이다.
		UDP 소켓을 기반으로 데이터 전송이 이뤄지므로 sendto 함수를 사용하는 것은 당연하다.
		그리고 sleep 함수 호출은 데이터 전송에 약간의 시간 간격을 두기 위함일 뿐, 별 다른 의미를 갖진 않는다.
	*/
	while (!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(hSendSock, buf, strlen(buf),
			0, (SOCKADDR*)&mulAdr, sizeof(mulAdr));
		Sleep(2000);
	}
	closesocket(hSendSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}