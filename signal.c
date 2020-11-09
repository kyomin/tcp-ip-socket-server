#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
	if (sig == SIGALRM)
		puts("Time out!");

	/*
		2초 간격으로 SIGALRM 시그널을 반복 발생시키기 위해 시그너 핸들러 내에서 alarm 함수를 호출하고 있다.
	*/
	alarm(2);
}

void keycontrol(int sig) {
	if (sig == SIGINT)
		puts("CTRL+C pressed");
}

int main(int argc, char *argv[]) {
	int i;
	
	/*
		시그널 등록
	*/
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);

	/*
		시그널 SIGALRM의 발생을 2초 뒤로 예약했다.
	*/
	alarm(2);

	for (i = 0; i < 3; i++) {
		puts("wait...");

		/*
			시그널의 발생과 시그널 핸들러의 실행을 확인하기 위해서 100초간 총 3회의 대기시간을 갖도록 반복문 내에서 sleep 함수를 호출하고 있다.
			그렇다면 이론상 총 300초, 대략 5분 정도가 지나야 프로그램이 종료된다.
			하지만 실제 실행시간을 보면 채 10초가 걸리지 않는다.

			그 이유는 시그널이 발생하면 sleep 함수의 호출로 블로킹 상태에 있던 프로세스가 깨어나기 때문이다!
			그리고 한 번 깨어나면 다시 잠들지 않는다.
		*/
		sleep(100);
	}
	return 0;
}