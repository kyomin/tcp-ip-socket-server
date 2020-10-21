#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
void ErrorHandling(char* message);

int main(int argc, char *argv[]) {
	WSADATA	wsaData;

	/*
		host_port와 host_addr에 각각 2바이트, 4바이트 데이터를 저장하고 있다.
		저장되는 바이트의 순서는 실행하는 CPU마다 차이를 보일 것이다.
	*/
	unsigned short host_port = 0x1234;
	unsigned short net_port;
	unsigned long host_addr = 0x12345678;
	unsigned long net_addr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	/*
		host_port와 host_addr에 저장된 데이터를 네트워크 바이트 순서로 변환하고 있다.
		만일 현재 코드를 실행하는 CPU가 리틀 엔디안 기준으로 정렬한다면,
		바이트 순서가 바뀐 값이 반환되어 변수에 저장된다.
	*/
	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("Host ordered port: %#x \n", host_port);
	printf("Network ordered port: %#x \n", net_port);
	printf("Host ordered address: %#lx \n", host_addr);
	printf("Network ordered address: %#lx \n", net_addr);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}