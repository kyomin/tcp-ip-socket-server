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
		�ñ׳� �߻� �� ȣ��� �Լ��� ����� ���ؼ��� �̷��� sigaction ����ü ������ �����ؼ�
		��� sa_handler�� �Լ� ������ ���� �����ؾ� �Ѵ�.
	*/
	struct sigaction act;
	act.sa_handler = timeout;
	
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	/*
		�ñ׳� SIGALRM�� ���� �ڵ鷯�� �����ϰ� �ִ�.
		�׸��� �̾ alarm �Լ� ȣ���� ���ؼ� 2�� �ڿ� �ñ׳� SIGALRM�� �߻��� ������ ���Ҵ�.
	*/
	sigaction(SIGALRM, &act, 0);

	alarm(2);

	for (i = 0; i < 3; i++) {
		puts("wait...");
		sleep(100);
	}
	return 0;
}