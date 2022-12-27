#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

using namespace sf;

int main(void) {

	// 윈도우창 생성
	RenderWindow window(VideoMode(640, 480), "AfterSchool");
	window.setFramerateLimit(60); // 60분에 1초 동안 반복문이 돌아감 빨리 가지 않게끔

	srand(time(0));

	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(50); // 글자크기 조절
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	text.setString("score");

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100); // 100, 100 기준으로 40 40 네모 그리기
	player.setFillColor(Color::Red); // 색깔 Red로 교체
	int player_speed = 5; // 속도
	int player_score = 0; // 점수

	RectangleShape enemy[5];
	int enemy_life[5];
	int enemy_score = 100; // 적을 잡을 때 얻는 함수
	// enemy 초기화
	for (int i = 0; i < 5; i++) {
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setFillColor(Color::Yellow);
		enemy_life[i] = 1;
		enemy[i].setPosition(rand()%300+300, rand()%480); // 300~599
	}

	// 윈도우가 열려있을 때까지 반복
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				// 종료(x) 버튼을 누르면 Event::Closed(0)
				case Event::Closed:
					window.close(); // 윈도우 닫기
					break;
					// 키보드를 눌렀을 때
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

		// 방향기 start
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			player.move(-player_speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			player.move(0, -player_speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			player.move(0, player_speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) { // else if 사용 X why? => if left top 두 개를 동시에 누르면?
			player.move(player_speed, 0);
		}   // 방향기 end

		// enemy와의 충돌
		for (int i = 0; i < 5; i++) {
			if (enemy_life[i] > 0) {
				if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds())) { // 겹치는 것이 있는지 판단
					printf("enemy[%d]과 충돌\n", i);
					enemy_life[i] -= 1;
					player_score += enemy_score;
				}
			}
		}

		printf("score : %d\n", player_score);

		window.clear(Color::Black);

		// draw는 나중에 호출할수록 우선순위가 높아짐
		for(int i=0; i<5; i++)
			if(enemy_life[i] > 0)
				window.draw(enemy[i]);
		window.draw(player);
		window.draw(text);

		window.display();
	}
	return 0;
}