#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
	if (sig == SIGALRM)
		puts("Time out!");
	alarm(2);
}

int main(int argc, char *argv[]) {
	int i;

	/*
		시그널 발생 시 호출될 함수의 등록을 위해서는 이렇듯 sigaction 구조체 변수를 선언해서
		멤버 sa_handler에 함수 포인터 값을 저장해야 한다.
	*/
	struct sigaction act;
	act.sa_handler = timeout;
	
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	/*
		시그널 SIGALRM에 대한 핸들러를 지정하고 있다.
		그리고 이어서 alarm 함수 호출을 통해서 2초 뒤에 시그널 SIGALRM의 발생을 예약해 놓았다.
	*/
	sigaction(SIGALRM, &act, 0);

	alarm(2);

	for (i = 0; i < 3; i++) {
		puts("wait...");
		sleep(100);
	}
	return 0;
}