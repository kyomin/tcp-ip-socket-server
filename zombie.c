#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	pid_t pid = fork();

	if (pid == 0)     // if Child Process
	{
		puts("Hi I'am a child process");
	}
	else
	{
		/*
			�ڽ� ���μ����� ID�� ����ϰ� �ִ�.
			�� ���� ���ؼ� �ڽ� ���μ����� ����(�������� �ƴ���)�� Ȯ���� �� �ִ�.
			�������� Ŀ��� â���� 'ps au' ��ɾ �Է��ϸ� ���¸� Ȯ���� �� �ִ�.
		*/
		printf("Child Process ID: %d \n", pid);

		/*
			30�ʰ� �θ� ���μ����� ���߱� ���� �ڵ��̴�.
			�θ� ���μ����� ����Ǹ� ���� ���¿� �ִ� �ڽ� ���μ����� �Բ� �Ҹ�Ǳ� ������,
			������ Ȯ���� ���ؼ��� �θ� ���μ����� ���Ḧ ������ų �ʿ䰡 �ִ�.
		*/
		sleep(30);     // Sleep 30 sec.
	}

	if (pid == 0)
		puts("End child process");
	else
		puts("End parent process");
	return 0;
}