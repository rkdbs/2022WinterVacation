#include <stdio.h>
#include <stdlib.h> // srand(), rand()
#include <time.h> // time()

void swap(int* pa, int *pb) {
	int temp = *pa;
	*pa = *pb;
	*pb = temp;
}

// _arr의 내용을 무작위로 섞는다.
void shuffle(int* _arr) {
	srand(time(0));
	for (int i = 0; i < 100; i++)
		swap(&_arr[rand() % 16], &_arr[rand() % 16]);
}

int main(void) {
	const int S = 4;
	int arr[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	
	shuffle(arr);

	for (int i = 0; i < 16; i++)
		printf("%d ", arr[i]);
}