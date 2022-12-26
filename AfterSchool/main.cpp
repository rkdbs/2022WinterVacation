#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {

	// ������â ����
	RenderWindow window(VideoMode(640, 480), "AfterSchool");

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100); // 100, 100 �������� 40 40 �׸� �׸���

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
		window.draw(player);
		window.display();
	}

	return 0;
}