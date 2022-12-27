#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.hpp>//SoundBuffer ���
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

    // 640 x 480 ������â ����
    // ��� ���ٰ� ������� �� return 0������ ���α׷��� ����� ��
    RenderWindow window(VideoMode(640, 480), "AfterSchool");
    window.setFramerateLimit(60); // 1�ʿ� 60�� �����ش�. �÷��̾ ���� ���� �ʵ��� �ϱ�

    srand((time(0)));
    
    long start_time = clock();      // ���� ���۽ð�
    long spent_time;            // ���� ����ð�
    
    // text
    Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"); // C����̺꿡 �ִ� ��Ʈ ��������

    Text text;
    char info[40];
    text.setFont(font); // ��Ʈ ����
    text.setCharacterSize(24); // ��Ʈ ũ��
    text.setFillColor(Color(255, 255, 255)); // RGB�� ��� ǥ��
    text.setPosition(0, 0); // �ؽ�Ʈ ��ġ 0,0

    Texture bg_texture;
    bg_texture.loadFromFile("./resources/images/background.jpg");
    Sprite bg_sprite;
    bg_sprite.setTexture(bg_texture);
    bg_sprite.setPosition(0, 0);

    // �÷��̾�
    struct Player player;
    player.sprite.setSize(Vector2f(40, 40)); // �÷��̾� ������
    player.sprite.setPosition(100, 100); // �÷��̾� ���� ��ġ
    player.sprite.setFillColor(Color::Red); // �÷��̾� ����
    player.speed = 5; // �÷��̾� �ӵ�
    player.score = 0; // �÷��̾� ����

    // ��(enemy)
    const int ENEMY_NUM = 10;
    
    struct Enemy enemy[ENEMY_NUM];

    // enemy �ʱ�ȭ
    for (int i = 0; i < ENEMY_NUM; i++)
    {
        // TODO : ������ ��ȿ������ �ڵ�
        enemy[i].explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
        enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
        enemy[i].score = 100;

        enemy[i].sprite.setSize(Vector2f(70, 70));
        enemy[i].sprite.setFillColor(Color::Yellow); // �� ����
        enemy[i].sprite.setPosition(rand() % 300 + 300, rand() % 380);
        enemy[i].life = 1;
        enemy[i].speed = -(rand() % 10 + 1);
    }


    // �����찡 �������� ������ �ݺ�, ���� ��Ű�� �����? -> ���� �ݺ�
    while (window.isOpen()) //������â�� �����ִ� ���� ��� �ݺ�
    {
        Event event;//�̺�Ʈ ����
        while (window.pollEvent(event)) //�̺�Ʈ�� �߻�. �̺�Ʈ�� �߻��ؾ� event �ʱ�ȭ�� ��
        {
            switch (event.type)
            {
                // ����(x)��ư�� ������ Event::Closed(0) 
            case Event::Closed: // ������
                window.close(); // ������â�� ����
                break;
                // Ű���带 ������ �� 
            case Event::KeyPressed:
                // case�� �ȿ� ������ ������ ������ �߰�ȣ�� �ľ� ��
            {
                // spaceŰ ������ ��� enemy �ٽ� ����
                if (event.key.code == Keyboard::Space)
                {
                    for (int i = 0; i < ENEMY_NUM; i++)
                    {
                        enemy[i].sprite.setSize(Vector2f(70, 70));
                        enemy[i].sprite.setFillColor(Color::Yellow);//�� ����
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

        // ����Ű start
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            player.sprite.move(-1 * player.speed, 0); // ���� �̵�
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            player.sprite.move(0, -1 * player.speed); // ���� �̵�
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            player.sprite.move(0, player.speed); // �Ʒ��� �̵�
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            player.sprite.move(player.speed, 0); // ������ �̵�
        } // ����� end

        // enemy���� �浹
        // intersects : �÷��̾�� �� ���̿��� �������� �ִ°�
        for (int i = 0; i < ENEMY_NUM; i++)
        {
            if (enemy[i].life > 0)
            {
                if (player.sprite.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds()))
                {
                    printf("enemy[%d]���� �浹\n", i);
                    enemy[i].life -= 1;//���� ���� ���̱�
                    player.score += enemy[i].score;

                    //TODO : �ڵ� refactoring �ʿ�
                    if (enemy[i].life == 0) {
                        enemy[i].explosion_sound.play();
                    }
                }
                enemy[i].sprite.move(enemy[i].speed, 0);
            }
        }

        sprintf(info, "score: %d  time: %d", player.score, spent_time / 1000); // �ǽð� ���� ����
        text.setString(info);

        window.clear(Color::Black);//�÷��̾� ��ü ���� (��� �����)
        window.draw(bg_sprite);

        for (int i = 0; i < ENEMY_NUM; i++)
        {
            if (enemy[i].life > 0)  window.draw(enemy[i].sprite);//�� �����ֱ�
        }
        //ȭ���� ������ �ִ� ���� ��� �׷��� ��
        //draw�� ���߿� ȣ���Ҽ��� �켱������ ������
        window.draw(player.sprite);//�÷��̾� �����ֱ�(�׷��ֱ�)
        window.draw(text);

        window.display();
    }

    return 0;
}