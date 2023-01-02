#include <stdio.h>

// _a와 a, _b와 b는 전혀 상관없는 변수
void swap(int _a, int _b) {
	int temp;
	temp = _a;
	_a = _b;
	_b = temp;
}

int main(void) {
	int a = 3;
	int b = 5;
	
	printf("a:%d b:%d\n", a, b);

	// a와 b를 swap하는 함수를 호출

	swap(a, b);

	printf("a:%d b:%d\n", a, b);
}