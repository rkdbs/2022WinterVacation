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
			switch (event.type) {
				// ����(x) ��ư�� ������ Event::Closed(0)
				case Event::Closed:
					window.close(); // ������ �ݱ�
			}
		}
	}

	return 0;
}