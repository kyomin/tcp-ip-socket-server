#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[]) {
	int fds[2];
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();

	if (pid == 0) {
		/*
			자식 프로세스의 실행영역이다.
			데이터를 전송도 하고, 데이터를 수신도 한다.
		*/
		write(fds[1], str1, sizeof(str1));
		sleep(2);
		read(fds[0], buf, BUF_SIZE);
		printf("Child proc output: %s \n", buf);
	}
	else {
		/*
			부모 프로세스의 실행 영역이다.
			read 함수를 통해 데이터를 수신하는데, 이는 자식 프로세스가 전송하는 데이터를 수신하기 위함이다.
			그리고 write 함수를 통해서 데이터를 전송하는데, 이는 자식 프로세스에게 수신이 된다.
		*/
		read(fds[0], buf, BUF_SIZE);
		printf("Parent proc output: %s \n", buf);
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}

	return 0;
}

/*
	위 함수의 문제점은 "파이프에 데이터가 전달되면, 먼저 가져가는 프로세스에게 이 데이터가 전달된다"는 것이다.

	쉽게 말해서 파이프에 데이터가 들어가면, 이는 임자가 없는 데이터가 된다.
	즉, read 함수 호출을 통해서 먼저 데이터를 읽어 들이는 프로세스에게 데이터가 전달된다.
	그것이 비록 파이프에 데이터를 가져다 놓은 프로세스라 하더라도 말이다.
	위의 sleep 함수로 제어를 하긴 하는데, 자식 프로세스 실행 영역에서 "sleep(2);"문을 넣지 않으면 파이프에 전달한 데이터를 자식 프로세스 본인이 다시 가져간다.
	결국 부모 프로세스는 read 함수를 호출하고 나서, 파이프에 데이터가 들어오기만을 기다리는 꼴이 된다.

	양방향으로 데이터를 송신하기 위해서는 파이프를 두 개 생성하는 것이 낫다.
	이 예제는 pipe3.c에 있다.
*/