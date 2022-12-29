/*
* 
* TODO list
* 1) 아이템 : 속도증가, 데미지 증가
* 2) 총알 시스템 개편
* 3) sound effect 개선
* 4) 총알 위치 체계적으로
*/
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.hpp> // SoundBuffer 사용
#include <time.h>
#include <SFML/Graphics.hpp>

using namespace sf;

struct Player {
    RectangleShape sprite;
    int speed;
    int score;
    int life;
    float x, y; // 플레이어 좌표
};

// 총알
struct Bullet {
    RectangleShape sprite;
    int is_fired;
};

struct Enemy {
    RectangleShape sprite;
    int speed;
    int life;
};

struct Item {
    RectangleShape sprite;
    int delay;
    int is_presented; // 아이템이 떴는지?
};

struct Textures {
    Texture bg; // 배경 이미지
    Texture enemy; // 적 이미지
    Texture gameover; // 게임오버 이미지
    Texture item_delay; // 공속 아이템 이미지
    Texture item_speed; // 이속 아이템 이미지
    Texture player; // 플레이어 이미지
};

struct SButters {
    SoundBuffer BGM;
    SoundBuffer rumble;
};

// obj1과 obj2 충돌여부
int is_collide(RectangleShape obj1, RectangleShape obj2) {
    return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// 전역변수
const int ENEMY_NUM = 7; // enemy의 최대개수
const int BULLET_NUM = 50; // bullet의 최대개수
const int W_WIDTH = 1200, W_HEIGHT = 600; // 창의 크기
const int GO_WIDTH = 320, GO_HEIGHT = 240; // 게임오버 그림의 크기

int main(void) {
    struct Textures t;
    t.bg.loadFromFile("./resources/images/background.jpg");
    t.enemy.loadFromFile("./resources/images/enemy.png");
    t.gameover.loadFromFile("./resources/images/gameover.png");
    t.item_delay.loadFromFile("./resources/images/item_delay.png");
    t.item_speed.loadFromFile("./resources/images/star.png");
    t.player.loadFromFile("./resources/images/earth.png");

    struct SButters sb;
    sb.BGM.loadFromFile("./resources/sounds/bgm.ogg");
    sb.rumble.loadFromFile("./resources/sounds/rumble.flac");

    // 640 x 480 윈도우창 생성
    // 잠깐 떴다가 사라지는 건 return 0때문에 프로그램이 종료된 것
    RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
    window.setFramerateLimit(60); // 1초에 60장 보여준다. 플레이어가 빨리 가지 않도록 하기

    srand((time(0)));
    
    long start_time = clock(); // 게임 시작시간
    long spent_time; // 게임 진행시간
    long fired_time = 0; // 최근에 발사한 시간
    int is_gameover = 0;

    // BGM
    Sound BGM_sound;
    BGM_sound.setBuffer(sb.BGM);
    BGM_sound.setVolume(80); // BGM 소리 줄이기 (기본값 100)
    BGM_sound.setLoop(1); // BGM 무한반복
    BGM_sound.play();

    // text
    Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"); // C드라이브에 있는 폰트 가져오기

    Text text;
    char info[40];
    text.setFont(font); // 폰트 세팅
    text.setCharacterSize(30); // 폰트 크기
    text.setFillColor(Color(255, 255, 255)); // RGB로 흰색 표현
    text.setPosition(0, 0); // 텍스트 위치 0,0

    // 배경
    Sprite bg_sprite;
    bg_sprite.setTexture(t.bg);
    bg_sprite.setPosition(0, 0);

    // gameover
    Sprite gameover_sprite;
    gameover_sprite.setTexture(t.gameover);
    gameover_sprite.setPosition((W_WIDTH - GO_WIDTH)/2, (W_HEIGHT - GO_HEIGHT)/2);

    // 플레이어
    struct Player player;
    player.sprite.setTexture(&t.player); // 주소값을 받기 위해 &
    player.sprite.setPosition(200, 200); // 플레이어 시작 위치
    player.sprite.setSize(Vector2f(100, 100));
    player.x = player.sprite.getPosition().x;
    player.y = player.sprite.getPosition().y;
    player.speed = 5; // 플레이어 속도
    player.score = 0; // 플레이어 점수
    player.life = 10; // 플레이어 목숨

    // 총알
    int bullet_speed = 20;
    int bullet_idx = 0;
    int bullet_delay = 500; // 딜레이 0.5초
    Sound bullet_sound;
    bullet_sound.setBuffer(sb.rumble);

    struct Bullet bullet[BULLET_NUM];
    for (int i = 0; i < BULLET_NUM; i++) {
        bullet[i].sprite.setSize(Vector2f(10, 10));
        bullet[i].sprite.setPosition(player.x + 50, player.y + 15); // 임시 테스트
    }

    // 적(enemy)
    struct Enemy enemy[ENEMY_NUM];
    Sound enemy_explosion_sound;
    enemy_explosion_sound.setBuffer(sb.rumble);
    int enemy_score = 100;
    int enemy_respwan_time = 8;

    // enemy 초기화
    for (int i = 0; i < ENEMY_NUM; i++) {
        enemy[i].sprite.setTexture(&t.enemy);
        enemy[i].sprite.setSize(Vector2f(70, 70));
        enemy[i].sprite.setScale(-1, 1); // 좌우대칭
        enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
        enemy[i].life = 1;
        enemy[i].speed = -(rand() % 10 + 1);
    }

    // item
    struct Item item[2];
    item[0].sprite.setTexture(&t.item_speed);
    item[0].delay = 25000; // 25초
    item[0].sprite.setSize(Vector2f(50, 50));
    item[0].is_presented = 1;

    // 윈도우가 열려있을 때까지 반복, 유지 시키는 방법은? -> 무한 반복
    while (window.isOpen()) { //윈도우창이 열려있는 동안 계속 반복
        spent_time = clock() - start_time;
        player.x = player.sprite.getPosition().x;
        player.y = player.sprite.getPosition().y;

        Event event; //이벤트 생성
        while (window.pollEvent(event)) { //이벤트가 발생. 이벤트가 발생해야 event 초기화가 됨
            switch (event.type) {
                // 종료(x)버튼을 누르면 Event::Closed(0) 
            case Event::Closed: // 정수임
                window.close(); // 윈도우창이 닫힘
                break;
                // 키보드를 눌렀을 때 
            //case Event::KeyPressed: { // case문 안에 변수를 선언할 때에는 중괄호를 쳐야 함
            //    if (event.key.code == Keyboard::Space) { // space키 누르면 모든 enemy 다시 출현
            //        for (int i = 0; i < 7; i++) {
            //            enemy[i].sprite.setSize(Vector2f(70, 70));
            //            enemy[i].sprite.setFillColor(Color::Yellow); //적 색상
            //            enemy[i].sprite.setPosition(rand() % 300 + 500, rand() % 380);
            //            enemy[i].life = 1;
            //            enemy[i].speed = -(rand() % 10 + 1);
            //        }
            //    }
            //}
            break;
        }
    }

    /* game 상태 update */
    if (player.life <= 0) {
        is_gameover = 1;
    }

    /* Player update */
    // 방향키 start
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        player.sprite.move(-1 * player.speed, 0); // 왼쪽 이동  
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        player.sprite.move(0, -1 * player.speed); // 위쪽 이동
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        player.sprite.move(0, player.speed); // 아래쪽 이동
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        player.sprite.move(player.speed, 0); // 오른쪽 이동
    } // 방향기 end

    // Player 이동범위 제한
    if (player.x < 0)
        player.sprite.setPosition(0, player.y);
    else if (player.x > W_WIDTH - 100) // 100(그림의 너비)
        player.sprite.setPosition(W_WIDTH - 100, player.y);

    if (player.y < 0)
        player.sprite.setPosition(player.x, 0);
    else if (player.y > W_HEIGHT - 100) // 100(그림의 높이)
        player.sprite.setPosition(player.x, W_HEIGHT - 100);

    /* Bullet update */
    printf("bullet_idx %d\n", bullet_idx);
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        if (spent_time-fired_time > bullet_delay) {
            // 총알이 발사되어있지 않다면
            if (!bullet[bullet_idx].is_fired) {
                bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
                bullet[bullet_idx].is_fired = 1;
                bullet_idx++; // 다음 총알이 발사할 수 있도록
                bullet_idx = bullet_idx % BULLET_NUM;
                bullet_sound.play();
                fired_time = spent_time; // 총알 장전
            }
        }
    }
    for (int i = 0; i < BULLET_NUM; i++) {
        if (bullet[i].is_fired) {
            bullet[i].sprite.move(bullet_speed, 0);
            if (bullet[i].sprite.getPosition().x > W_WIDTH)
                bullet[i].is_fired = 0;
        }
    }

    /* Enemy update */
    for (int i = 0; i < ENEMY_NUM; i++) {
         // 10초마다 enemy가 젠
         if (spent_time % (1000 * enemy_respwan_time) < 1000 / 60 + 1) {
             // 게임이 진행중일 때만 적을 리스폰 시키겠다.
             if (!is_gameover) {
                 enemy[i].sprite.setSize(Vector2f(70, 70));
                 enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9 , rand() % 380);
                 enemy[i].life = 1;
                 // 10초마다 enemy의 속도 + 1
                 enemy[i].speed = -(rand() % 10 + 1 + (spent_time/1000/ enemy_respwan_time));
             }
         }
        
         if (enemy[i].life > 0) {
             // player, enemy 충돌
             if (is_collide(player.sprite, enemy[i].sprite)) { // intersects : 플레이어와 적 사이에서 교집합이 있는가
                    enemy[i].life -= 1; // 적의 생명 줄이기
                    player.score += enemy_score;

                    // TODO : 코드 refactoring 필요
                    if (enemy[i].life == 0) {
                        enemy_explosion_sound.play();
                    }
                }
                // 적이 왼쪽 끝에 진입하려는 순간
                else if (enemy[i].sprite.getPosition().x < 0) {
                    player.life -= 1;
                    enemy[i].life = 0;
                }
                
                // 총알과 enemy의 충돌
                for (int j = 0; j < BULLET_NUM; j++) {
                     if (is_collide(bullet[j].sprite, enemy[i].sprite)) {
                         if (bullet[j].is_fired) {
                             enemy[i].life -= 1;
                             player.score += enemy_score;

                             if (enemy[i].life == 0) {
                                 enemy_explosion_sound.play();
                             }
                             bullet[j].is_fired = 0;
                         }
                     }
                }
                enemy[i].sprite.move(enemy[i].speed, 0);
            }
        }

        // item update
        if (item[0].is_presented) {
            // TODO : 충돌시 아이템 효과를 주고 사라진다
        }
        
        sprintf(info, "life : %d score : %d  time : %d", player.life, player.score, spent_time / 1000); // 실시간 점수 변경
        text.setString(info);

        window.clear(Color::Black); // 플레이어 자체 제거 (배경 지우기)

        // 화면이 열려져 있는 동안 계속 그려야 함
        // draw는 나중에 호출할수록 우선순위가 높아짐
        window.draw(bg_sprite);
        for (int i = 0; i < ENEMY_NUM; i++)
            if (enemy[i].life > 0)
                window.draw(enemy[i].sprite); // 적 보여주기
        if (item[0].is_presented)
            window.draw(item[0].sprite);
        window.draw(player.sprite); // 플레이어 보여주기(그려주기)
        window.draw(text);
        for (int i = 0; i < BULLET_NUM; i++) {
            if (bullet[i].is_fired) {
                window.draw(bullet[i].sprite);
            }
        }

        if (is_gameover) {
            // TODO : 게임이 멈추는 것을 구현할 것
            window.draw(gameover_sprite);
        }

        window.display();
    }

    return 0;
}