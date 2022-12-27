#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.hpp>//SoundBuffer 사용
#include <time.h>
#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

struct Player {
    RectangleShape sprite;
    int speed;
    int score;
};

struct Enemy {
    RectangleShape sprite;
    int speed;
    int life;
    int score;
    SoundBuffer explosion_buffer;
    Sound explosion_sound;
};

int main(void) {

    // 640 x 480 윈도우창 생성
    // 잠깐 떴다가 사라지는 건 return 0때문에 프로그램이 종료된 것
    RenderWindow window(VideoMode(640, 480), "AfterSchool");
    window.setFramerateLimit(60); // 1초에 60장 보여준다. 플레이어가 빨리 가지 않도록 하기

    srand((time(0)));
    
    long start_time = clock();      // 게임 시작시간
    long spent_time;            // 게임 진행시간
    
    // text
    Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"); // C드라이브에 있는 폰트 가져오기

    Text text;
    char info[40];
    text.setFont(font); // 폰트 세팅
    text.setCharacterSize(24); // 폰트 크기
    text.setFillColor(Color(255, 255, 255)); // RGB로 흰색 표현
    text.setPosition(0, 0); // 텍스트 위치 0,0

    Texture bg_texture;
    bg_texture.loadFromFile("./resources/images/background.jpg");
    Sprite bg_sprite;
    bg_sprite.setTexture(bg_texture);
    bg_sprite.setPosition(0, 0);

    // 플레이어
    struct Player player;
    player.sprite.setSize(Vector2f(40, 40)); // 플레이어 사이즈
    player.sprite.setPosition(100, 100); // 플레이어 시작 위치
    player.sprite.setFillColor(Color::Red); // 플레이어 색상
    player.speed = 5; // 플레이어 속도
    player.score = 0; // 플레이어 점수

    // 적(enemy)
    const int ENEMY_NUM = 10;
    
    struct Enemy enemy[ENEMY_NUM];

    // enemy 초기화
    for (int i = 0; i < ENEMY_NUM; i++)
    {
        // TODO : 굉장히 비효율적인 코드
        enemy[i].explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
        enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
        enemy[i].score = 100;

        enemy[i].sprite.setSize(Vector2f(70, 70));
        enemy[i].sprite.setFillColor(Color::Yellow); // 적 색상
        enemy[i].sprite.setPosition(rand() % 300 + 300, rand() % 380);
        enemy[i].life = 1;
        enemy[i].speed = -(rand() % 10 + 1);
    }


    // 윈도우가 열려있을 때까지 반복, 유지 시키는 방법은? -> 무한 반복
    while (window.isOpen()) //윈도우창이 열려있는 동안 계속 반복
    {
        Event event;//이벤트 생성
        while (window.pollEvent(event)) //이벤트가 발생. 이벤트가 발생해야 event 초기화가 됨
        {
            switch (event.type)
            {
                // 종료(x)버튼을 누르면 Event::Closed(0) 
            case Event::Closed: // 정수임
                window.close(); // 윈도우창이 닫힘
                break;
                // 키보드를 눌렀을 때 
            case Event::KeyPressed:
                // case문 안에 변수를 선언할 때에는 중괄호를 쳐야 함
            {
                // space키 누르면 모든 enemy 다시 출현
                if (event.key.code == Keyboard::Space)
                {
                    for (int i = 0; i < ENEMY_NUM; i++)
                    {
                        enemy[i].sprite.setSize(Vector2f(70, 70));
                        enemy[i].sprite.setFillColor(Color::Yellow);//적 색상
                        enemy[i].sprite.setPosition(rand() % 300 + 300, rand() % 410);
                        enemy[i].life = 1;
                        enemy[i].speed = -(rand() % 10 + 1);
                    }
                }
                break;
            }
            }
        }
        spent_time = clock() - start_time;

        // 방향키 start
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            player.sprite.move(-1 * player.speed, 0); // 왼쪽 이동
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            player.sprite.move(0, -1 * player.speed); // 위쪽 이동
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            player.sprite.move(0, player.speed); // 아래쪽 이동
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            player.sprite.move(player.speed, 0); // 오른쪽 이동
        } // 방향기 end

        // enemy와의 충돌
        // intersects : 플레이어와 적 사이에서 교집합이 있는가
        for (int i = 0; i < ENEMY_NUM; i++)
        {
            if (enemy[i].life > 0)
            {
                if (player.sprite.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds()))
                {
                    printf("enemy[%d]와의 충돌\n", i);
                    enemy[i].life -= 1;//적의 생명 줄이기
                    player.score += enemy[i].score;

                    //TODO : 코드 refactoring 필요
                    if (enemy[i].life == 0) {
                        enemy[i].explosion_sound.play();
                    }
                }
                enemy[i].sprite.move(enemy[i].speed, 0);
            }
        }

        sprintf(info, "score: %d  time: %d", player.score, spent_time / 1000); // 실시간 점수 변경
        text.setString(info);

        window.clear(Color::Black);//플레이어 자체 제거 (배경 지우기)
        window.draw(bg_sprite);

        for (int i = 0; i < ENEMY_NUM; i++)
        {
            if (enemy[i].life > 0)  window.draw(enemy[i].sprite);//적 보여주기
        }
        //화면이 열려져 있는 동안 계속 그려야 함
        //draw는 나중에 호출할수록 우선순위가 높아짐
        window.draw(player.sprite);//플레이어 보여주기(그려주기)
        window.draw(text);

        window.display();
    }

    return 0;
}