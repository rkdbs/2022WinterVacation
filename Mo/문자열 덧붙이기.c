#include <stdio.h>
#include <string.h> // ���ڿ� ���̺귯��
#include <stdlib.h> // itoa ���

int main(void) {
	char str[30] = "score : ";
	int score = 300;

	char str2[30] = "300";
	// ������ ���ڿ��� (��ȯ�� ����, ������, ����)
	itoa(score, str2, 10);

	strcatx(str, str2); // str�� str2 �߰�
	printf("%s", str);

	return 0;
}