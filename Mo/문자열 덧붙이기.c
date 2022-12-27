#include <stdio.h>
#include <string.h> // 문자열 라이브러리

int main(void) {
	char str[30] = "score : ";
	char str2[30] = "300";

	strcatx(str, str2); // str에 str2 추가
	printf("%s", str);

	int score = 300;

	return 0;
}