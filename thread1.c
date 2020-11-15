#include <stdio.h>
#include <pthread.h>
void* thread_main(void *arg);

int main(int argc, char *argv[]) {
	pthread_t t_id;
	int thread_param = 5;

	/*
		thread_main 함수의 호출을 시작으로 별도의 실행흐름을 구성하는 쓰레드의 생성을 요청하고 있다.
		더불어 thread_main 함수 호출 시 인자로 변수 thread_param의 주소 값을 전달하고 있다.
	*/
	if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0) {
		puts("pthread_create() error");
		return -1;
	};

	/*
		sleep 함수의 호출을 통해서 main 함수의 실행을 10초간 중지시키고 있다.
		이는 프로세스의 종료시기를 늦추기 위함이다.
		여기 프로세스의 return문이 실행되면 프로세스는 종료된다.
		그리고 프로세스의 종료는 그 안에서 생성된 쓰레드의 종료로 이어진다.
		따라서 쓰레드의 실행을 보장하기 위해서 이 문장이 삽입되었다.
	*/
	sleep(10);  
	puts("end of main");
	
	return 0;
}

/*
	매개변수 arg로 전달되는 것은 위의 main 함수에서 호출한 pthread_create 함수의 네 번째 전달인자이다.
*/
void* thread_main(void *arg) {
	int i;
	int cnt = *((int*)arg);
	for (i = 0; i < cnt; i++) {
		sleep(1);  puts("running thread");
	}
	return NULL;
}