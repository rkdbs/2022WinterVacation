#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace sf;

const int S = 4;

struct Card {
	RectangleShape sprite;
	int id_i;
	int id_j;
	int type;
	int is_clicked; // 현재 뒤집혀진 카드인지
	int is_cleared; // 정답을 맞춘 카드인지
};

struct SButters {
	SoundBuffer BGM;
	SoundBuffer TRUE;
	SoundBuffer FALSE;
};

struct Textures {
	Texture gameover;
};

void swap_card(struct Card* c1, struct Card* c2) {
	struct Card temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}

int main(void) {
	struct Textures p;
	p.gameover.loadFromFile("./resources/images/gameover.png");

	int CARD_W = 200;
	int CARD_H = 200;

	RenderWindow window(VideoMode(800, 800), "AfterSchool2");
	window.setFramerateLimit(60);

	Vector2i mouse_pos; // i : 마우스 좌표(정수)
	int flipped_num = 0; // 마우스 누른 횟수

	long start_time; // 시작 시각
	long spent_time; // 현재 시각
	long delay_time; // 바로 원래로 뒤집혀지지 않도록 딜레이를 줌, 딜레이 시작 시각

	srand(time(0));

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
	text.setFillColor(Color::White);
	text.setPosition(0, 0);
	char info[40];

	struct SButters sb;
	sb.TRUE.loadFromFile("./resources/sounds/true.wav");
	sb.FALSE.loadFromFile("./resources/sounds/false.ogg");

	Sound TRUE_sound;
	TRUE_sound.setBuffer(sb.TRUE);
	// TRUE_sound.setVolume(50);

	Sound FALSE_sound;
	FALSE_sound.setBuffer(sb.FALSE);
	// FALSE_sound.setVolume(40);

	Sprite gameover_sprite;
	gameover_sprite.setTexture(p.gameover);
	gameover_sprite.setPosition(250, 300);

	struct Card compare_card; // 첫 번째로 뒤집힌 카드
	struct Card cards[S][S];
	int n = 0;
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < S; j++) {
			cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
			cards[i][j].sprite.setPosition(j * CARD_W, i * CARD_H);
			cards[i][j].sprite.setTexture(&t[0]);
			cards[i][j].type = 1 + n / 2;
			cards[i][j].is_clicked = 0; 
			cards[i][j].is_cleared = 0;
			n++;
		}
	}

	// 카드 100번 섞기
	for (int i = 0; i < 100; i++) {
		swap_card(&(cards[rand() % S][rand() % S]), &(cards[rand() % S][rand() % S])); // 순서는 바꾸지만 멤버변수 값 바꾸지 X
	}

	// idx에 맞춰서 id값 재조정
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < S; j++) {
			cards[i][j].id_i = i;
			cards[i][j].id_j = j;
			cards[i][j].sprite.setPosition(j * CARD_W, i * CARD_H);
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

										// 뒤집혀진 카드가 한 개라면
										if (flipped_num == 1) {
											compare_card = cards[i][j];
										}
										// 두 개를 뒤집었다면
										else if (flipped_num == 2) {
											// 두 카드가 같은 종류이면
											if (compare_card.type == cards[i][j].type) {
												cards[i][j].is_cleared = 1;
												cards[compare_card.id_i][compare_card.id_j].is_cleared = 1;
												TRUE_sound.play();
											}
											// 두 카드가 다른 종류이면
											else {
												delay_time = spent_time;
												FALSE_sound.play();
											}
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
				// 클릭이 된 상태거나, 정답을 맞춘 카드이면	
				if (cards[i][j].is_clicked == 1 || cards[i][j].is_cleared == 1)
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

		sprintf(info, "TIME : %d\n", spent_time/1000);
		text.setString(info);

		window.clear(Color::Black);
		for (int i = 0; i < S; i++) {
			for (int j = 0; j < S; j++) {
				window.draw(cards[i][j].sprite);
			}
		}

		window.draw(text);

		if (spent_time / 1000 >= 50) {
			window.draw(gameover_sprite);
		}

		window.display();
	}

	return 0;
}