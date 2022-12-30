#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace sf;

const int S = 4;

struct Card {
	RectangleShape sprite;
	int id;
};

int main(void) {
	int CARD_W = 200;
	int CARD_H = 200;

	RenderWindow window(VideoMode(1200, 800), "AfterSchool2");
	window.setFramerateLimit(60);

	Vector2i mouse_pos; // i : ���콺 ��ǥ(����)
	int click_cnt = 0; // ���콺 ���� Ƚ��

	Font font;
	font.loadFromFile("c:/Windows/Fonts/arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::White);
	text.setPosition(0, 0);
	char info[40];

	struct Card cards[5][5];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < S; j++) {
			cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
			cards[i][j].sprite.setPosition(j * CARD_W, i * CARD_H);
			cards[i][j].sprite.setFillColor(Color(i * 120, j * 120, 0));
		}
	}

	while (window.isOpen()) {
		mouse_pos = Mouse::getPosition(window);

		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				window.close();
				break;
			// �� �� ������ �� ���� ����
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left) {
					click_cnt++;
				}
			}
		}

		sprintf(info, "(%d, %d) click %d\n", mouse_pos.x, mouse_pos.y, click_cnt);
		text.setString(info);

		window.clear(Color::Black);
		for (int i = 0; i < S; i++) {
			for (int j = 0; j < S; j++) {
				window.draw(cards[i][j].sprite);
			}
		}

		window.draw(text);

		window.display();
	}

	return 0;
}