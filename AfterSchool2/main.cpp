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
	int is_clicked; // ���� �������� ī������
	int is_cleared; // ������ ���� ī������
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

	Vector2i mouse_pos; // i : ���콺 ��ǥ(����)
	int flipped_num = 0; // ���콺 ���� Ƚ��

	long start_time; // ���� �ð�
	long spent_time; // ���� �ð�
	long delay_time; // �ٷ� ������ ���������� �ʵ��� �����̸� ��, ������ ���� �ð�

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

	struct Card compare_card; // ù ��°�� ������ ī��
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

	// ī�� 100�� ����
	for (int i = 0; i < 100; i++) {
		swap_card(&(cards[rand() % S][rand() % S]), &(cards[rand() % S][rand() % S])); // ������ �ٲ����� ������� �� �ٲ��� X
	}

	// idx�� ���缭 id�� ������
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
			// �� �� ������ �� ���� ����
			// TODO : ��ư ������ ���� �ȵǴ� ���� �ذ��ϱ�
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left) {
					for (int i = 0; i < S; i++) {
						for (int j = 0; j < S; j++) {
							if (flipped_num < 2) {
								// ���콺 ��ġ�� cards[i][j]�� ��ġ�� �ش��Ѵٸ�?
								if (cards[i][j].sprite.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
									// ���������� ���� ī�常 �����ڴ�.
									if (cards[i][j].is_clicked == 0) {
										cards[i][j].is_clicked = 1;
										flipped_num++;

										// �������� ī�尡 �� �����
										if (flipped_num == 1) {
											compare_card = cards[i][j];
										}
										// �� ���� �������ٸ�
										else if (flipped_num == 2) {
											// �� ī�尡 ���� �����̸�
											if (compare_card.type == cards[i][j].type) {
												cards[i][j].is_cleared = 1;
												cards[compare_card.id_i][compare_card.id_j].is_cleared = 1;
												TRUE_sound.play();
											}
											// �� ī�尡 �ٸ� �����̸�
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
				// Ŭ���� �� ���°ų�, ������ ���� ī���̸�	
				if (cards[i][j].is_clicked == 1 || cards[i][j].is_cleared == 1)
					// �׸��� �ִ� ��������Ʈ�� ����(ī�带 �����ڴٴ� �ǹ�)
					cards[i][j].sprite.setTexture(&t[cards[i][j].type]);
				else
					// ī��� ??? ����
					cards[i][j].sprite.setTexture(&t[0]);
			}
		}
		
		// ������ ī�尡 2�����
		if (flipped_num == 2) {
			// �� ī�尡 �������� 1�� �̳����
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