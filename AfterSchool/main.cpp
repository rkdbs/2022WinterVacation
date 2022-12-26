#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {

	// ������â ����
	RenderWindow window(VideoMode(640, 480), "AfterSchool");
	window.setFramerateLimit(60); // 60�п� 1�� ���� �ݺ����� ���ư� ���� ���� �ʰԲ�

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100); // 100, 100 �������� 40 40 �׸� �׸���
	player.setFillColor(Color::Red); // ���� Red�� ��ü
	int player_speed = 5;

	RectangleShape enemy;
	enemy.setSize(Vector2f(70, 70));
	enemy.setPosition(500, 300);
	enemy.setFillColor(Color::Yellow);

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

		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			player.move(-player_speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			player.move(0, -player_speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			player.move(0, player_speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) { // else if ��� X why? => if left top �� ���� ���ÿ� ������?
			player.move(player_speed, 0);
		}

		if (player.getGlobalBounds().intersects(enemy.getGlobalBounds())) { // ��ġ�� ���� �ִ��� �Ǵ�
			printf("enemy�� �浹\n");
		}

		window.clear(Color::Black);

		// draw�� ���߿� ȣ���Ҽ��� �켱������ ������
		window.draw(enemy);
		window.draw(player);

		window.display();
	}

	return 0;
}