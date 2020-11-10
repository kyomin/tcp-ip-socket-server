#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	/*
		좀비 프로세스의 생성을 막기 위한 코드 구성
	*/
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while (1) {
		adr_sz = sizeof(clnt_adr);

		/*
			클라이언트 요청이 오면 해당 클라이언트와의 소켓을 생성한다.
		*/
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		
		if (clnt_sock == -1)
			continue;
		else
			puts("new client connected...");
		
		/*
			자식 프로세스가 만들어지면서 accept를 통해 생성된 클라이언트 소켓 정보를 복사해 갖게 된다.
			즉, 따로 넘길 필요가 없다.
			여기서 메모리의 복사가 이뤄지기 때문이다.
		*/
		pid = fork();

		if (pid == -1) {
			close(clnt_sock);
			continue;
		}
		
		/*
			자식 프로세스의 실행 영역이다.
			이 부분에 의해서 클라이언트에게 에코 서비스가 제공된다.
			그런데 "close(serv_sock);" 문장으로 서버 소켓을 닫고 있다.
			이는 자식 프로세스가 서버 소켓의 파일 디스크립터까지 복사하기 때문이다.
			이에 대한 자세한 이론은 README.md 참고!
		*/
		if (pid == 0) {
			close(serv_sock);
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				write(clnt_sock, buf, str_len);

			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void read_childproc(int sig) {
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed proc id: %d \n", pid);
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}