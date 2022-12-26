#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

using namespace sf;

int main(void) {

	// ������â ����
	RenderWindow window(VideoMode(640, 480), "AfterSchool");
	window.setFramerateLimit(60); // 60�п� 1�� ���� �ݺ����� ���ư� ���� ���� �ʰԲ�

	srand(time(0));

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100); // 100, 100 �������� 40 40 �׸� �׸���
	player.setFillColor(Color::Red); // ���� Red�� ��ü
	int player_speed = 5;

	RectangleShape enemy[5];
	int enemy_life[5];
	for (int i = 0; i < 5; i++) {
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setFillColor(Color::Yellow);
		enemy_life[i] = 1;
		enemy[i].setPosition(rand()%300+300, rand()%480); // 300~599
	}

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

		// enemy���� �浹
		for (int i = 0; i < 5; i++) {
			if (enemy_life[i] > 0) {
				if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds())) { // ��ġ�� ���� �ִ��� �Ǵ�
					printf("enemy[%d]�� �浹\n", i);
					enemy_life[i] -= 1;
				}
			}
		}

		window.clear(Color::Black);

		// draw�� ���߿� ȣ���Ҽ��� �켱������ ������
		for(int i=0; i<5; i++)
			if(enemy_life[i] > 0)
				window.draw(enemy[i]);
		window.draw(player);

		window.display();
	}
	return 0;
}