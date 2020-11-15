#include <stdio.h>
#include <windows.h>
#include <process.h>
#define NUM_THREAD	100

void * thread_inc(void * arg);
void * thread_des(void * arg);

/*
	젼역변수 num은 쓰레드가 공유하는 데이터 영역에 놓인다.
*/
long long num = 0;

int main(int argc, char *argv[]) {
	HANDLE thread_id[NUM_THREAD];
	int i;

	printf("sizeof long long: %d \n", sizeof(long long));
	for (i = 0; i < NUM_THREAD; i++) {
		if (i % 2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}

	for (i = 0; i < NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);

	printf("result: %lld \n", num);
	return 0;
}

void * thread_inc(void * arg) {
	int i;
	for (i = 0; i < 50000000; i++)
		num += 1;
	return NULL;
}

void * thread_des(void * arg) {
	int i;
	for (i = 0; i < 50000000; i++)
		num -= 1;
	return NULL;
}

/*
	위 예제에서는 총 100개의 쓰레드를 생성해서 그 중 반은 thread_inc를 쓰레드의 main 함수로,
	나머지 반은 thread_des를 쓰레드의 main 함수로 호출하게 하고 있다.
	
	이로써 전역변수 num에 저장된 값의 증가와 감소의 최종결과로 변수 num에는 0이 저장되어야 한다.
	그런데 코드를 돌려보면 알겠지만, 실행결과는 0이 아니다!
	뿐만 아니라 실행할 때마다 매번 그 결과도 다르다.
	이는 '임계영역'과 관련이 있는 문제이다.
*/