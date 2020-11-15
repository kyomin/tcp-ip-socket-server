#include <stdio.h>
#include <pthread.h>
void* thread_main(void *arg);

int main(int argc, char *argv[]) {
	pthread_t t_id;
	int thread_param = 5;

	/*
		thread_main �Լ��� ȣ���� �������� ������ �����帧�� �����ϴ� �������� ������ ��û�ϰ� �ִ�.
		���Ҿ� thread_main �Լ� ȣ�� �� ���ڷ� ���� thread_param�� �ּ� ���� �����ϰ� �ִ�.
	*/
	if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0) {
		puts("pthread_create() error");
		return -1;
	};

	/*
		sleep �Լ��� ȣ���� ���ؼ� main �Լ��� ������ 10�ʰ� ������Ű�� �ִ�.
		�̴� ���μ����� ����ñ⸦ ���߱� �����̴�.
		���� ���μ����� return���� ����Ǹ� ���μ����� ����ȴ�.
		�׸��� ���μ����� ����� �� �ȿ��� ������ �������� ����� �̾�����.
		���� �������� ������ �����ϱ� ���ؼ� �� ������ ���ԵǾ���.
	*/
	sleep(10);  
	puts("end of main");
	
	return 0;
}

/*
	�Ű����� arg�� ���޵Ǵ� ���� ���� main �Լ����� ȣ���� pthread_create �Լ��� �� ��° ���������̴�.
*/
void* thread_main(void *arg) {
	int i;
	int cnt = *((int*)arg);
	for (i = 0; i < cnt; i++) {
		sleep(1);  puts("running thread");
	}
	return NULL;
}