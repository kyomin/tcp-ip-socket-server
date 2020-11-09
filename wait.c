#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int status;
	pid_t pid = fork();

	if (pid == 0) {
		// 자식 프로세스 종료 1
		return 3;
	}
	else {
		printf("Child PID: %d \n", pid);
		pid = fork();
		if (pid == 0) {
			// 자식 프로세스 종료 2
			exit(7);
		}
		else {		// 부모 프로세스 실행 영역
			printf("Child PID: %d \n", pid);
			/*
				이 함수로 인해서 종료된 프로세스 관련 정보는 status에 담기게 되고, 
				해당 정보의 프로세스는 완전히 소멸된다.
			*/
			wait(&status);
			
			/*
				WIFEXITED 매크로를 이용해 자식 프로세스의 정상 종료 여부를 확인하고 있다.
				그리고 정상 종료인 경우에 한해서 WEXITSTATUS 매크로 함수를 호출하여 자식 프로세스가 전달한 값을 출력하고 있다.
			*/
			if (WIFEXITED(status))
				printf("Child send one: %d \n", WEXITSTATUS(status));

			/*
				앞서 생성한 자식 프로세스가 두 개이므로 또 한 번의 wait 함수 호출과 매크로 함수의 호출을 진행하고 있다.
			*/
			wait(&status);
			if (WIFEXITED(status))
				printf("Child send two: %d \n", WEXITSTATUS(status));

			/*
				부모 프로세스의 종료를 잠시 멈추기 위한 코드이다.
				이 순간에 자식 프로세스의 상태를 확인하면 된다.
			*/
			sleep(30);     // Sleep 30 sec.
		}
	}
	return 0;
}