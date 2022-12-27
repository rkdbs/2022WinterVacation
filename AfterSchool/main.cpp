#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

int main(void) {

	// ������â ����
	RenderWindow window(VideoMode(640, 480), "AfterSchool");
	window.setFramerateLimit(60); // 60�п� 1�� ���� �ݺ����� ���ư� ���� ���� �ʰԲ�

	srand(time(0));

	long start_time = clock(); // ���� ���۽ð�
	long spent_time; // ���� ����ð�

	// text
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30); // ����ũ�� ����
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	char info[40];
	text.setString("score");

	// ���
	Texture bg_texture;
	bg_texture.loadFromFile("./resources/images/background.jpg");
	Sprite bg_sprite;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);

	// �÷��̾�
	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100); // 100, 100 �������� 40 40 �׸� �׸���
	player.setFillColor(Color::Red); // ���� Red�� ��ü
	int player_speed = 5; // �ӵ�
	int player_score = 0; // ����

	// ��(enemy)
	RectangleShape enemy[5];
	int enemy_life[5];
	int enemy_score = 100; // ���� ���� �� ��� �Լ�
	SoundBuffer enemy_explosion_buffer;
	enemy_explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(enemy_explosion_buffer);

	// enemy �ʱ�ȭ
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
					break;
					// Ű���带 ������ ��
				case Event::KeyPressed: {
					if (event.key.code == Keyboard::Space) {
						for (int i = 0; i < 5; i++) {
							enemy[i].setSize(Vector2f(70, 70));
							enemy[i].setFillColor(Color::Yellow);
							enemy_life[i] = 1;
							enemy[i].setPosition(rand() % 300 + 300, rand() % 480); // 300~599
						}
					}
					break;
				}
			}
		}

		spent_time = clock() - start_time;

		// ����� start
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
		}   // ����� end

		// enemy���� �浹
		for (int i = 0; i < 5; i++) {
			if (enemy_life[i] > 0) {
				if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds())) { // ��ġ�� ���� �ִ��� �Ǵ�
					printf("enemy[%d]�� �浹\n", i);
					enemy_life[i] -= 1;
					player_score += enemy_score;

					if (enemy_life[i] == 0) {
						enemy_explosion_sound.play();
					}
				}
			}
		}

		sprintf(info, "score : %d time : %d\n", player_score, spent_time/1000); // �ǽð����� ������ ����Ǳ� ����
		text.setString(info);

		window.clear(Color::Black);
		window.draw(bg_sprite);

		// draw�� ���߿� ȣ���Ҽ��� �켱������ ������
		for(int i=0; i<5; i++)
			if(enemy_life[i] > 0)
				window.draw(enemy[i]);
		window.draw(player);
		window.draw(text);

		window.display();
	}
	return 0;
}