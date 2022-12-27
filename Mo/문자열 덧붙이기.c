#include <stdio.h>
#include <string.h> // 문자열 라이브러리
#include <stdlib.h> // itoa 사용

int main(void) {
	char str[30] = "score : ";
	int score = 300;

	char str2[30] = "300";
	// 정수를 문자열로 (반환할 정수, 적용대상, 진수)
	itoa(score, str2, 10);

	strcatx(str, str2); // str에 str2 추가
	printf("%s", str);

	return 0;
}