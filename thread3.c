#include <stdio.h>
#include <pthread.h>
void * thread_summation(void * arg);

/*
	전역 변수 => 쓰레드가 공유하는 데이터 영역에 놓인다.
*/
int sum = 0;

int main(int argc, char *argv[]) {
	pthread_t id_t1, id_t2;
	int range1[] = { 1, 5 };
	int range2[] = { 6, 10 };

	/*
		두 개의 쓰레드를 생성하고 있다.
	*/
	pthread_create(&id_t1, NULL, thread_summation, (void *)range1);
	pthread_create(&id_t2, NULL, thread_summation, (void *)range2);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);
	printf("result: %d \n", sum);
	return 0;
}

void * thread_summation(void * arg) {
	int start = ((int*)arg)[0];
	int end = ((int*)arg)[1];

	while (start <= end) {
		/*
			두 개의 쓰레드가 하나의 전역변수 sum에 동시에 접근하고 있다.
		*/
		sum += start;
		start++;
	}

	return NULL;
}