#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[]) {
	int fds1[2], fds2[2];
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	/*
		파이프를 2개 생성하고 있다.
	*/
	pipe(fds1), pipe(fds2);
	pid = fork();

	/*
		자식 -> 부모 : fds1
		부모 -> 자식 : fds2
	*/
	if (pid == 0) {
		write(fds1[1], str1, sizeof(str1));
		read(fds2[0], buf, BUF_SIZE);
		printf("Child proc output: %s \n", buf);
	}
	else {
		read(fds1[0], buf, BUF_SIZE);
		printf("Parent proc output: %s \n", buf);
		write(fds2[1], str2, sizeof(str2));
		sleep(3);
	}

	return 0;
}