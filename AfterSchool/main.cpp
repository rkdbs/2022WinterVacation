#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {

	// ������â ����
	RenderWindow window(VideoMode(640, 480), "AfterSchool");

	// �����찡 �������� ������ �ݺ�
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			// ����(x) ��ư�� ������
			if (event.type == Event::Closed)
				window.close(); // â �ݱ�
		}
	}

	return 0;
}