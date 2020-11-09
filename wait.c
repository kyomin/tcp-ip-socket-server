#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int status;
	pid_t pid = fork();

	if (pid == 0) {
		// �ڽ� ���μ��� ���� 1
		return 3;
	}
	else {
		printf("Child PID: %d \n", pid);
		pid = fork();
		if (pid == 0) {
			// �ڽ� ���μ��� ���� 2
			exit(7);
		}
		else {		// �θ� ���μ��� ���� ����
			printf("Child PID: %d \n", pid);
			/*
				�� �Լ��� ���ؼ� ����� ���μ��� ���� ������ status�� ���� �ǰ�, 
				�ش� ������ ���μ����� ������ �Ҹ�ȴ�.
			*/
			wait(&status);
			
			/*
				WIFEXITED ��ũ�θ� �̿��� �ڽ� ���μ����� ���� ���� ���θ� Ȯ���ϰ� �ִ�.
				�׸��� ���� ������ ��쿡 ���ؼ� WEXITSTATUS ��ũ�� �Լ��� ȣ���Ͽ� �ڽ� ���μ����� ������ ���� ����ϰ� �ִ�.
			*/
			if (WIFEXITED(status))
				printf("Child send one: %d \n", WEXITSTATUS(status));

			/*
				�ռ� ������ �ڽ� ���μ����� �� ���̹Ƿ� �� �� ���� wait �Լ� ȣ��� ��ũ�� �Լ��� ȣ���� �����ϰ� �ִ�.
			*/
			wait(&status);
			if (WIFEXITED(status))
				printf("Child send two: %d \n", WEXITSTATUS(status));

			/*
				�θ� ���μ����� ���Ḧ ��� ���߱� ���� �ڵ��̴�.
				�� ������ �ڽ� ���μ����� ���¸� Ȯ���ϸ� �ȴ�.
			*/
			sleep(30);     // Sleep 30 sec.
		}
	}
	return 0;
}