#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig) {
	int status;
	pid_t id = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status)) {
		printf("Removed proc id: %d \n", id);
		printf("Child send: %d \n", WEXITSTATUS(status));
	}
}

int main(int argc, char *argv[]) {
	pid_t pid;

	/*
		시그널 SIGCHLD에 대한 시그널 핸들러의 등록 과정을 보이고 있다.
		이로써 자식 프로세스가 종료되면 read_childproc 함수가 호출된다.
		그리고 이 함수 내에서의 waitpid 함수 호출로 인해 자식 프로세스는 좀비가 되지 않고 소멸된다.
	*/
	struct sigaction act;
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, 0);

	pid = fork();
	if (pid == 0) {
		puts("Hi! I'm child process");
		sleep(10);
		return 12;
	}
	else {
		printf("Child proc id: %d \n", pid);
		pid = fork();
		if (pid == 0) {
			puts("Hi! I'm child process");
			sleep(10);
			exit(24);
		}
		else {
			int i;
			printf("Child proc id: %d \n", pid);
			
			for (i = 0; i < 5; i++) {
				puts("wait...");
				sleep(5);
			}
		}
	}

	return 0;
}

/*

--- 실행 결과 ---

root@my_linux:/home/swyoon/tcpip# gcc remove_zombie.c -o zombie
root@my_linux:/home/swyoon/tcpip# ./zombie
Hi! I'm child process
Child proc id: 9529
Hi! I'm child process
Child proc id: 9530
wait...
wait...
Removed proc id: 9530
Child send: 24
wait...
Removed proc id: 9529
Child send: 12
wait...
wait...

*/