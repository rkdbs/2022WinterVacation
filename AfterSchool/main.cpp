/*
* 
* TODO list
* 1) ������ : �ӵ�����, ������ ����
* 2) �Ѿ� �ý��� ����
* 3) sound effect ����
* 4) �Ѿ� ��ġ ü��������
*/
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.hpp> // SoundBuffer ���
#include <time.h>
#include <SFML/Graphics.hpp>

using namespace sf;

struct Player {
    RectangleShape sprite;
    int speed;
    int score;
    int life;
    float x, y; // �÷��̾� ��ǥ
};

// �Ѿ�
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
    int is_presented; // �������� ������?
};

struct Textures {
    Texture bg; // ��� �̹���
    Texture enemy; // �� �̹���
    Texture gameover; // ���ӿ��� �̹���
    Texture item_delay; // ���� ������ �̹���
    Texture item_speed; // �̼� ������ �̹���
    Texture player; // �÷��̾� �̹���
};

struct SButters {
    SoundBuffer BGM;
    SoundBuffer rumble;
};

// obj1�� obj2 �浹����
int is_collide(RectangleShape obj1, RectangleShape obj2) {
    return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// ��������
const int ENEMY_NUM = 7; // enemy�� �ִ밳��
const int BULLET_NUM = 50; // bullet�� �ִ밳��
const int W_WIDTH = 1200, W_HEIGHT = 600; // â�� ũ��
const int GO_WIDTH = 320, GO_HEIGHT = 240; // ���ӿ��� �׸��� ũ��

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

    // 640 x 480 ������â ����
    // ��� ���ٰ� ������� �� return 0������ ���α׷��� ����� ��
    RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
    window.setFramerateLimit(60); // 1�ʿ� 60�� �����ش�. �÷��̾ ���� ���� �ʵ��� �ϱ�

    srand((time(0)));
    
    long start_time = clock(); // ���� ���۽ð�
    long spent_time; // ���� ����ð�
    long fired_time = 0; // �ֱٿ� �߻��� �ð�
    int is_gameover = 0;

    // BGM
    Sound BGM_sound;
    BGM_sound.setBuffer(sb.BGM);
    BGM_sound.setVolume(80); // BGM �Ҹ� ���̱� (�⺻�� 100)
    BGM_sound.setLoop(1); // BGM ���ѹݺ�
    BGM_sound.play();

    // text
    Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"); // C����̺꿡 �ִ� ��Ʈ ��������

    Text text;
    char info[40];
    text.setFont(font); // ��Ʈ ����
    text.setCharacterSize(30); // ��Ʈ ũ��
    text.setFillColor(Color(255, 255, 255)); // RGB�� ��� ǥ��
    text.setPosition(0, 0); // �ؽ�Ʈ ��ġ 0,0

    // ���
    Sprite bg_sprite;
    bg_sprite.setTexture(t.bg);
    bg_sprite.setPosition(0, 0);

    // gameover
    Sprite gameover_sprite;
    gameover_sprite.setTexture(t.gameover);
    gameover_sprite.setPosition((W_WIDTH - GO_WIDTH)/2, (W_HEIGHT - GO_HEIGHT)/2);

    // �÷��̾�
    struct Player player;
    player.sprite.setTexture(&t.player); // �ּҰ��� �ޱ� ���� &
    player.sprite.setPosition(200, 200); // �÷��̾� ���� ��ġ
    player.sprite.setSize(Vector2f(100, 100));
    player.x = player.sprite.getPosition().x;
    player.y = player.sprite.getPosition().y;
    player.speed = 5; // �÷��̾� �ӵ�
    player.score = 0; // �÷��̾� ����
    player.life = 10; // �÷��̾� ���

    // �Ѿ�
    int bullet_speed = 20;
    int bullet_idx = 0;
    int bullet_delay = 500; // ������ 0.5��
    Sound bullet_sound;
    bullet_sound.setBuffer(sb.rumble);

    struct Bullet bullet[BULLET_NUM];
    for (int i = 0; i < BULLET_NUM; i++) {
        bullet[i].sprite.setSize(Vector2f(10, 10));
        bullet[i].sprite.setPosition(player.x + 50, player.y + 15); // �ӽ� �׽�Ʈ
    }

    // ��(enemy)
    struct Enemy enemy[ENEMY_NUM];
    Sound enemy_explosion_sound;
    enemy_explosion_sound.setBuffer(sb.rumble);
    int enemy_score = 100;
    int enemy_respwan_time = 8;

    // enemy �ʱ�ȭ
    for (int i = 0; i < ENEMY_NUM; i++) {
        enemy[i].sprite.setTexture(&t.enemy);
        enemy[i].sprite.setSize(Vector2f(70, 70));
        enemy[i].sprite.setScale(-1, 1); // �¿��Ī
        enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
        enemy[i].life = 1;
        enemy[i].speed = -(rand() % 10 + 1);
    }

    // item
    struct Item item[2];
    item[0].sprite.setTexture(&t.item_speed);
    item[0].delay = 25000; // 25��
    item[0].sprite.setSize(Vector2f(50, 50));
    item[0].is_presented = 1;

    // �����찡 �������� ������ �ݺ�, ���� ��Ű�� �����? -> ���� �ݺ�
    while (window.isOpen()) { //������â�� �����ִ� ���� ��� �ݺ�
        spent_time = clock() - start_time;
        player.x = player.sprite.getPosition().x;
        player.y = player.sprite.getPosition().y;

        Event event; //�̺�Ʈ ����
        while (window.pollEvent(event)) { //�̺�Ʈ�� �߻�. �̺�Ʈ�� �߻��ؾ� event �ʱ�ȭ�� ��
            switch (event.type) {
                // ����(x)��ư�� ������ Event::Closed(0) 
            case Event::Closed: // ������
                window.close(); // ������â�� ����
                break;
                // Ű���带 ������ �� 
            //case Event::KeyPressed: { // case�� �ȿ� ������ ������ ������ �߰�ȣ�� �ľ� ��
            //    if (event.key.code == Keyboard::Space) { // spaceŰ ������ ��� enemy �ٽ� ����
            //        for (int i = 0; i < 7; i++) {
            //            enemy[i].sprite.setSize(Vector2f(70, 70));
            //            enemy[i].sprite.setFillColor(Color::Yellow); //�� ����
            //            enemy[i].sprite.setPosition(rand() % 300 + 500, rand() % 380);
            //            enemy[i].life = 1;
            //            enemy[i].speed = -(rand() % 10 + 1);
            //        }
            //    }
            //}
            break;
        }
    }

    /* game ���� update */
    if (player.life <= 0) {
        is_gameover = 1;
    }

    /* Player update */
    // ����Ű start
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        player.sprite.move(-1 * player.speed, 0); // ���� �̵�  
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        player.sprite.move(0, -1 * player.speed); // ���� �̵�
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        player.sprite.move(0, player.speed); // �Ʒ��� �̵�
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        player.sprite.move(player.speed, 0); // ������ �̵�
    } // ����� end

    // Player �̵����� ����
    if (player.x < 0)
        player.sprite.setPosition(0, player.y);
    else if (player.x > W_WIDTH - 100) // 100(�׸��� �ʺ�)
        player.sprite.setPosition(W_WIDTH - 100, player.y);

    if (player.y < 0)
        player.sprite.setPosition(player.x, 0);
    else if (player.y > W_HEIGHT - 100) // 100(�׸��� ����)
        player.sprite.setPosition(player.x, W_HEIGHT - 100);

    /* Bullet update */
    printf("bullet_idx %d\n", bullet_idx);
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        if (spent_time-fired_time > bullet_delay) {
            // �Ѿ��� �߻�Ǿ����� �ʴٸ�
            if (!bullet[bullet_idx].is_fired) {
                bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
                bullet[bullet_idx].is_fired = 1;
                bullet_idx++; // ���� �Ѿ��� �߻��� �� �ֵ���
                bullet_idx = bullet_idx % BULLET_NUM;
                bullet_sound.play();
                fired_time = spent_time; // �Ѿ� ����
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
         // 10�ʸ��� enemy�� ��
         if (spent_time % (1000 * enemy_respwan_time) < 1000 / 60 + 1) {
             // ������ �������� ���� ���� ������ ��Ű�ڴ�.
             if (!is_gameover) {
                 enemy[i].sprite.setSize(Vector2f(70, 70));
                 enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9 , rand() % 380);
                 enemy[i].life = 1;
                 // 10�ʸ��� enemy�� �ӵ� + 1
                 enemy[i].speed = -(rand() % 10 + 1 + (spent_time/1000/ enemy_respwan_time));
             }
         }
        
         if (enemy[i].life > 0) {
             // player, enemy �浹
             if (is_collide(player.sprite, enemy[i].sprite)) { // intersects : �÷��̾�� �� ���̿��� �������� �ִ°�
                    enemy[i].life -= 1; // ���� ���� ���̱�
                    player.score += enemy_score;

                    // TODO : �ڵ� refactoring �ʿ�
                    if (enemy[i].life == 0) {
                        enemy_explosion_sound.play();
                    }
                }
                // ���� ���� ���� �����Ϸ��� ����
                else if (enemy[i].sprite.getPosition().x < 0) {
                    player.life -= 1;
                    enemy[i].life = 0;
                }
                
                // �Ѿ˰� enemy�� �浹
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
            // TODO : �浹�� ������ ȿ���� �ְ� �������
        }
        
        sprintf(info, "life : %d score : %d  time : %d", player.life, player.score, spent_time / 1000); // �ǽð� ���� ����
        text.setString(info);

        window.clear(Color::Black); // �÷��̾� ��ü ���� (��� �����)

        // ȭ���� ������ �ִ� ���� ��� �׷��� ��
        // draw�� ���߿� ȣ���Ҽ��� �켱������ ������
        window.draw(bg_sprite);
        for (int i = 0; i < ENEMY_NUM; i++)
            if (enemy[i].life > 0)
                window.draw(enemy[i].sprite); // �� �����ֱ�
        if (item[0].is_presented)
            window.draw(item[0].sprite);
        window.draw(player.sprite); // �÷��̾� �����ֱ�(�׷��ֱ�)
        window.draw(text);
        for (int i = 0; i < BULLET_NUM; i++) {
            if (bullet[i].is_fired) {
                window.draw(bullet[i].sprite);
            }
        }

        if (is_gameover) {
            // TODO : ������ ���ߴ� ���� ������ ��
            window.draw(gameover_sprite);
        }

        window.display();
    }

    return 0;
}