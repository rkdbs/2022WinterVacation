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
	int type;
	int is_clicked;
};

int main(void) {
	int CARD_W = 200;
	int CARD_H = 200;

	RenderWindow window(VideoMode(1200, 800), "AfterSchool2");
	window.setFramerateLimit(60);

	Vector2i mouse_pos; // i : 마우스 좌표(정수)
	int flipped_num = 0; // 마우스 누른 횟수

	long start_time; // 시작 시각
	long spent_time; // 현재 시각
	long delay_time; // 바로 원래로 뒤집혀지지 않도록 딜레이를 줌, 딜레이 시작 시각

	Texture t[8 + 1];
	t[0].loadFromFile("./resources/images/back.jpg");
	t[1].loadFromFile("./resources/images/ca0.jpg");
	t[2].loadFromFile("./resources/images/ca1.jpg");
	t[3].loadFromFile("./resources/images/ca2.jpg");
	t[4].loadFromFile("./resources/images/ca3.jpg");
	t[5].loadFromFile("./resources/images/ca4.jpg");
	t[6].loadFromFile("./resources/images/ca5.jpg");
	t[7].loadFromFile("./resources/images/ca6.jpg");
	t[8].loadFromFile("./resources/images/ca7.jpg");

	Font font;
	font.loadFromFile("c:/Windows/Fonts/arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::Blue);
	text.setPosition(0, 0);
	char info[40];

	struct Card cards[S][S];
	int n = 0;
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < S; j++) {
			cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
			cards[i][j].sprite.setPosition(j * CARD_W, i * CARD_H);
			cards[i][j].sprite.setTexture(&t[0]);
			cards[i][j].type = 1 + n / 2;
			cards[i][j].id = n + 1;
			cards[i][j].is_clicked = 0;
			n++;
		}
	}

	start_time = clock();
	delay_time = start_time;

	while (window.isOpen()) {
		mouse_pos = Mouse::getPosition(window);
		spent_time = clock() - start_time;

		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				window.close();
				break;
			// 한 번 누르면 한 번만 적용
			// TODO : 버튼 눌러도 적용 안되는 버그 해결하기
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left) {
					for (int i = 0; i < S; i++) {
						for (int j = 0; j < S; j++) {
							if (flipped_num < 2) {
								// 마우스 위치가 cards[i][j]의 위치에 해당한다면?
								if (cards[i][j].sprite.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
									// 뒤집혀지지 않은 카드만 뒤집겠다.
									if (cards[i][j].is_clicked == 0) {
										cards[i][j].is_clicked = 1;
										flipped_num++;
										// 두 개를 뒤집었다면
										if (flipped_num == 2) {
											delay_time = spent_time;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		for (int i = 0; i < S; i++) {
			for (int j = 0; j < S; j++) {
				// 클릭이 된 상태면
				if (cards[i][j].is_clicked == 1)
					// 그림이 있는 스프라이트로 변경(카드를 뒤집겠다는 의미)
					cards[i][j].sprite.setTexture(&t[cards[i][j].type]);
				else
					// 카드는 ??? 상태
					cards[i][j].sprite.setTexture(&t[0]);
			}
		}
		
		// 뒤집한 카드가 2개라면
		if (flipped_num == 2) {
			// 두 카드가 뒤집힌지 1초 이내라면
			if(spent_time - delay_time <= 1000) {
			}
			else {
				for (int i = 0; i < S; i++)
					for (int j = 0; j < S; j++)
						cards[i][j].is_clicked = 0;
				flipped_num = 0;
			}
		}

		sprintf(info, "(%d, %d) clicks %d %d\n", mouse_pos.x, mouse_pos.y, spent_time/1000,delay_time/1000);
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