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
    int speed;
    int is_fired;
    int move;
};

struct Enemy {
    RectangleShape sprite;
    int speed;
    int life;
    int score;
    SoundBuffer explosion_buffer;
    Sound explosion_sound;
    int respawn_time;
};

struct Textures {
    Texture bg; // ��� �̹���
    Texture enemy; // �� �̹���
    Texture gameover; // ���ӿ��� �̹���
    Texture player; // �÷��̾� �̹���
};

// obj1�� obj2 �浹����
int is_collide(RectangleShape obj1, RectangleShape obj2) {
    return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// ��������
const int ENEMY_NUM = 7;
const int W_WIDTH = 1200, W_HEIGHT = 600; // â�� ũ��
const int GO_WIDTH = 320, GO_HEIGHT = 240; // ���ӿ��� �׸��� ũ��

int main(void) {
    struct Textures t;
    t.bg.loadFromFile("./resources/images/background.jpg");
    t.enemy.loadFromFile("./resources/images/enemy.png");
    t.gameover.loadFromFile("./resources/images/gameover.jpg");
    t.player.loadFromFile("./resources/images/earth.png");

    // 640 x 480 ������â ����
    // ��� ���ٰ� ������� �� return 0������ ���α׷��� ����� ��
    RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
    window.setFramerateLimit(60); // 1�ʿ� 60�� �����ش�. �÷��̾ ���� ���� �ʵ��� �ϱ�

    srand((time(0)));
    
    long start_time = clock(); // ���� ���۽ð�
    long spent_time; // ���� ����ð�
    int is_gameover = 0;

    // BGM
    SoundBuffer BGM_buffer;
    BGM_buffer.loadFromFile("./resources/sounds/bgm.ogg");
    Sound BGM_sound;
    BGM_sound.setBuffer(BGM_buffer);
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
    player.sprite.setPosition(100, 100); // �÷��̾� ���� ��ġ
    player.sprite.setSize(Vector2f(100, 100));
    player.x = player.sprite.getPosition().x;
    player.y = player.sprite.getPosition().y;
    player.speed = 5; // �÷��̾� �ӵ�
    player.score = 0; // �÷��̾� ����
    player.life = 10; // �÷��̾� ���

    // �Ѿ�
    struct Bullet bullet;
    bullet.sprite.setSize(Vector2f(10, 10));
    bullet.sprite.setPosition(player.x + 50, player.y + 15); // �ӽ� �׽�Ʈ
    bullet.speed = 20;
    bullet.is_fired = 0;

    // ��(enemy)
    struct Enemy enemy[ENEMY_NUM];

    // enemy �ʱ�ȭ
    for (int i = 0; i < ENEMY_NUM; i++) {
        // TODO : ������ ��ȿ������ �ڵ�
        enemy[i].explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
        enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
        enemy[i].score = 100;
        enemy[i].respawn_time = 7;

        enemy[i].sprite.setTexture(&t.enemy);
        enemy[i].sprite.setSize(Vector2f(70, 70));
        enemy[i].sprite.setScale(-1, 1); // �¿��Ī
        enemy[i].sprite.setFillColor(Color::Yellow); // �� ����
        enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
        enemy[i].life = 1;
        enemy[i].speed = -(rand() % 10 + 1);
    }

    // �����찡 �������� ������ �ݺ�, ���� ��Ű�� �����? -> ���� �ݺ�
    while (window.isOpen()) { //������â�� �����ִ� ���� ��� �ݺ�
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
    spent_time = clock() - start_time;
    player.x = player.sprite.getPosition().x;
    player.y = player.sprite.getPosition().y;

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

    // �Ѿ� �߻�
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        // �Ѿ��� �߻�Ǿ����� �ʴٸ�
        if (!bullet.is_fired) {
            bullet.sprite.setPosition(player.x + 50, player.y + 15);
            bullet.is_fired = 1;
        }
    }

    for (int i = 0; i < ENEMY_NUM; i++) {
         // 10�ʸ��� enemy�� ��
         if (spent_time % (1000*enemy[i].respawn_time) < 100 / 60 + 1) {
             enemy[i].sprite.setSize(Vector2f(70, 70));
             enemy[i].sprite.setFillColor(Color::Yellow); // �� ����
             enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH, rand() % 380);
             enemy[i].life = 1;
             // 10�ʸ��� enemy�� �ӵ� +1
             enemy[i].speed = -(rand() % 10 + 1 + (spent_time/1000/enemy[i].respawn_time));
         }
        
         if (enemy[i].life > 0) {
             // player, enemy �浹
             if (is_collide(player.sprite, enemy[i].sprite)) { // intersects : �÷��̾�� �� ���̿��� �������� �ִ°�
                    enemy[i].life -= 1; // ���� ���� ���̱�
                    player.score += enemy[i].score;

                    // TODO : �ڵ� refactoring �ʿ�
                    if (enemy[i].life == 0) {
                        enemy[i].explosion_sound.play();
                    }
                }
                // ���� ���� ���� �����Ϸ��� ����
                else if (enemy[i].sprite.getPosition().x < 0) {
                    player.life -= 1;
                    enemy[i].life = 0;
                }
                
                // �Ѿ˰� enemy�� �浹
                 if (is_collide(bullet.sprite, enemy[i].sprite)) {
                     enemy[i].life -= 1;
                     player.score += enemy[i].score;

                     // TODO �ڵ� : refactoring �ʿ�
                     if (enemy[i].life == 0) {
                         enemy[i].explosion_sound.play();
                     }
                     bullet.is_fired = 0;
                 }
                 enemy[i].sprite.move(enemy[i].speed, 0);
            }
        }
        
        // TODO : �Ѿ��� ��� �� ���� �߻�Ǵ� ���׸� �����ϱ�
        if (bullet.is_fired) {
            bullet.sprite.move(bullet.speed, 0);
            if (bullet.sprite.getPosition().x > W_WIDTH)
                bullet.is_fired = 0;
        }

        if (player.life <= 0) {
            is_gameover = 1;
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
        window.draw(player.sprite); // �÷��̾� �����ֱ�(�׷��ֱ�)
        window.draw(text);
        if (bullet.is_fired) {
            window.draw(bullet.sprite);
        }
        window.draw(bullet.sprite);

        if (is_gameover) {
            // TODO : ������ ���ߴ� ���� ������ ��
            window.draw(gameover_sprite);
        }

        window.display();
    }

    return 0;
}