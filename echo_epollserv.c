#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(char *buf);

int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len, i;
	char buf[BUF_SIZE];

	struct epoll_event *ep_events;
	struct epoll_event event;
	int epfd, event_cnt;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	epfd = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

	/*
		수신할 데이터가 존재하는 상황의 이벤트 등록
	*/
	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	while (1) {
		/*
			해당 이벤트가 발생한 파일 디스크립터의 개수
		*/
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		
		if (event_cnt == -1) {
			puts("epoll_wait() error");
			break;
		}

		for (i = 0; i < event_cnt; i++) {
			/*
				수신된 데이터가 서버 소켓에 있다면, 클라이언트 소켓을 등록해야 한다.
			*/
			if (ep_events[i].data.fd == serv_sock) {
				adr_sz = sizeof(clnt_adr);
				clnt_sock =
					accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
				event.events = EPOLLIN;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				printf("connected client: %d \n", clnt_sock);
			}
			/*
				수신된 데이터가 클라이언트와 연결된 소켓에 있다면,
				이에 따른 에코 서비스 처리를 진행한다.
			*/
			else {
				str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
				if (str_len == 0) {		// close request!
					epoll_ctl(
						epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					printf("closed client: %d \n", ep_events[i].data.fd);
				}
				else {
					write(ep_events[i].data.fd, buf, str_len);    // echo!
				}
			}
		}
	}
	close(serv_sock);
	close(epfd);
	return 0;
}

void error_handling(char *buf) {
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}