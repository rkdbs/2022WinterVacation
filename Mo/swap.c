#include <stdio.h>

// _a�� a, _b�� b�� ���� ������� ����
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

	// a�� b�� swap�ϴ� �Լ��� ȣ��

	swap(a, b);

	printf("a:%d b:%d\n", a, b);
}