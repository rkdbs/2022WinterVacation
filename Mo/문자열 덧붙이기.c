#include <stdio.h>
#include <string.h> // ���ڿ� ���̺귯��

int main(void) {
	char str[30] = "score : ";
	// str�� ���� �� ���ϱ�
	int str_len = strlen(str);

	char str2[30] = "300";
	// str2�� ���� �� ���ϱ�
	int str2_len = strlen(str2);

	for (int i = 0; i < str2_len; i++)
		str[i + str_len] = str2[i];
	str[str_len + str2_len] = '\0'; // ������ ���� �׻� null ��
	printf("%s", str);

	int score = 300;

	return 0;
}