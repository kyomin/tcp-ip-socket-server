#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	/*
		fd_set�� ���� reads�� �ʱ�ȭ�ϰ�, ���� ��ũ���� 0�� ��ġ�� 1�� �������ְ� �ִ�.
		��, ǥ�� �Է¿� ��ȭ�� �ִ��� ������ �ΰ� ���ڴٴ� ���̴�.
	*/
	FD_ZERO(&reads);
	FD_SET(0, &reads); // 0 is standard input(console)

	/*
		�� �κп��� �ּ�ó�� �� ������ �����غ���.
		�̴� select �Լ��� Ÿ�Ӿƿ� ������ ���� �ڵ��̴�.
		�׷��� �� ��ġ���� Ÿ�Ӿƿ��� �����ϸ� �� �ȴ�.
		�ֳ��ϸ� select �Լ� ȣ�� �Ŀ��� ����ü timeval�� ��� tv_sec�� tv_usec�� ����� ���� Ÿ�Ӿƿ��� �߻��ϱ���� ���Ҵ� �ð����� �ٲ�� �����̴�.
		���� select �Լ� ȣ�� ���� �Ź� timeval ����ü ������ �ʱ�ȭ�� �ݺ��ؾ� �Ѵ�.
	*/
	/*
	timeout.tv_sec=5;
	timeout.tv_usec=5000;
	*/

	while (1)
	{
		/*
			fd_set�� ���� reads�� ������ ���� temps�� �����ϰ� �ִµ�, ���⿡�� �׷����� ������ �ִ�.
			select �Լ� ȣ���� ������ ��ȭ�� ���� ���� ��ũ������ ��ġ�� ������ ������ ��ġ�� ��Ʈ���� 0���� �ʱ�ȭ�ȴ�.
			���� ������ ������ ���ؼ��� �̷��� ������ ������ ���ľ� �Ѵ�.
		*/
		temps = reads;

		/*
			�̷��� timeval ����ü ������ �ʱ�ȭ �ڵ帣�� �ݺ��� �ȿ� �����ؼ� select �Լ��� ȣ��Ǳ� ���� �Ź� ���Ӱ� ���� �ʱ�ȭȸ���� �ؾ� �Ѵ�.
		*/
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		/*
			select �Լ��� ȣ���ϰ� �ִ�.
			�ַܼκ��� �Էµ� �����Ͱ� �ִٸ� 0���� ū ���� ��ȯ�Ǹ�,
			�Էµ� �����Ͱ� ��� Ÿ�Ӿƿ��� �߻��ϴ� ��쿡�� 0�� ��ȯ�ȴ�.
		*/
		result = select(1, &temps, 0, 0, &timeout);

		if (result == -1) {
			puts("select() error!");
			break;
		}
		else if (result == 0) {
			puts("Time-out!");
		}
		else {
			/*
				select �Լ��� 0���� ū ���� ��ȯ���� �� ����Ǵ� �����̴�.
				��ȭ�� ���� ���� ��ũ���Ͱ� ǥ���Է��� �´��� Ȯ���ϰ�, ������ ǥ���Է����κ��� �����͸� �о �ַܼ� �����͸� ����ϰ� �ִ�.
			*/
			if (FD_ISSET(0, &temps)) {
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console: %s", buf);
			}
		}
	}

	return 0;
}