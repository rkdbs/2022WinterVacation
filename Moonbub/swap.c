#include <stdio.h>

// _a와 a, _b와 b는 전혀 상관없는 변수, 따라서 a, b값은 바뀌지 않음.
void swap(int* _a, int* _b) {
	int temp;
	temp = *_a;
	*_a = *_b;
	*_b = temp;
}

int main(void) {
	int a = 3;
	int b = 5;
	
	printf("a:%d b:%d\n", a, b);

	// a와 b를 swap하는 함수를 호출

	swap(&a, &b); // _a에게 a의 주소값을, _b에게 b의 주소값을 넘겨준다

	printf("a:%d b:%d\n", a, b);
}