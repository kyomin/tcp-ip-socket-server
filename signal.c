#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
	if (sig == SIGALRM)
		puts("Time out!");

	/*
		2�� �������� SIGALRM �ñ׳��� �ݺ� �߻���Ű�� ���� �ñ׳� �ڵ鷯 ������ alarm �Լ��� ȣ���ϰ� �ִ�.
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
		�ñ׳� ���
	*/
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);

	/*
		�ñ׳� SIGALRM�� �߻��� 2�� �ڷ� �����ߴ�.
	*/
	alarm(2);

	for (i = 0; i < 3; i++) {
		puts("wait...");

		/*
			�ñ׳��� �߻��� �ñ׳� �ڵ鷯�� ������ Ȯ���ϱ� ���ؼ� 100�ʰ� �� 3ȸ�� ���ð��� ������ �ݺ��� ������ sleep �Լ��� ȣ���ϰ� �ִ�.
			�׷��ٸ� �̷л� �� 300��, �뷫 5�� ������ ������ ���α׷��� ����ȴ�.
			������ ���� ����ð��� ���� ä 10�ʰ� �ɸ��� �ʴ´�.

			�� ������ �ñ׳��� �߻��ϸ� sleep �Լ��� ȣ��� ���ŷ ���¿� �ִ� ���μ����� ����� �����̴�!
			�׸��� �� �� ����� �ٽ� ����� �ʴ´�.
		*/
		sleep(100);
	}
	return 0;
}