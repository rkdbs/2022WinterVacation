#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {

	// 윈도우창 생성
	RenderWindow window(VideoMode(640, 480), "AfterSchool");
	window.setFramerateLimit(60); // 60분에 1초 동안 반복문이 돌아감 빨리 가지 않게끔

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100); // 100, 100 기준으로 40 40 네모 그리기
	player.setFillColor(Color::Red); // 색깔 Red로 교체
	int player_speed = 5;

	RectangleShape enemy;
	enemy.setSize(Vector2f(70, 70));
	enemy.setPosition(500, 300);
	enemy.setFillColor(Color::Yellow);

	// 윈도우가 열려있을 때까지 반복
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				// 종료(x) 버튼을 누르면 Event::Closed(0)
				case Event::Closed:
					window.close(); // 윈도우 닫기
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
		if (Keyboard::isKeyPressed(Keyboard::Right)) { // else if 사용 X why? => if left top 두 개를 동시에 누르면?
			player.move(player_speed, 0);
		}

		if (player.getGlobalBounds().intersects(enemy.getGlobalBounds())) { // 겹치는 것이 있는지 판단
			printf("enemy과 충돌\n");
		}

		window.clear(Color::Black);

		// draw는 나중에 호출할수록 우선순위가 높아짐
		window.draw(enemy);
		window.draw(player);

		window.display();
	}

	return 0;
}