#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void * read(void * arg);
void * accu(void * arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char *argv[]) {
	pthread_t id_t1, id_t2;

	/*
		세마포어 두 개를 생성하고 있다.
		하나는 세마포어 값이 0이고, 다른 하나는 1이다.
		이 두 개의 세마포어가 필요한 이유를 잘 이해해야 한다.
	*/
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);

	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	return 0;
}

void * read(void * arg) {
	int i;

	for (i = 0; i < 5; i++) {
		fputs("Input num: ", stdout);

		/*
			sem_wait(&sem_two); 문장에 의해 sem_two의 값은 1에서 0이 된다.
			때문에 accu의 sem_post(&sem_two); 문장이 호출돼서 1이 될 때까지 여기서 블로킹된다.
			즉, 순서가 맞춰진다.
		*/
		sem_wait(&sem_two);
		scanf("%d", &num);
		sem_post(&sem_one);
	}

	return NULL;
}

void * accu(void * arg) {
	int sum = 0, i;
	
	for (i = 0; i < 5; i++) {

		/*
			만일 초기에 위 read 함수에 앞서 이 부분에 접근 시,
			sem_one의 값이 0이므로 블로킹 상태에 놓이게 된다.
			위 read 함수의 sem_post(&sem_one); 문장이 실행 돼서 sem_one의 값을 1 증가시킬 때까지 말이다.
		*/
		sem_wait(&sem_one);
		sum += num;
		sem_post(&sem_two);
	}

	printf("Result: %d \n", sum);
	
	return NULL;
}