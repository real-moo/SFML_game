#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>      //str()
#include <ctime>        //time()
#include <cstdlib>      //srand(), rand()
#include <cmath>        //fabs()
#include <unistd.h>     //sleep()
//#include <Window.h>    //window에서 Sleep()

#include "ResourcePath.hpp"     //resource 경로
#include "Alien.hpp"

#define WIDTH 1000      //화면 가로 길이
#define HEIGHT 1200     //화면 세로 길이
#define ALIENWIDTH 900  //외계인이 화면 밖으로 나가지 않기 위해 선언

using namespace std;
using namespace sf;

int main() {
    srand(time(NULL));
    //SFML 윈도우를 열어주고 타이틀을 정해줌
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "GALAGA");
    window.setFramerateLimit(60); //프레임을 60으로 정해줌

    //비행기
    Texture t1;
    t1.loadFromFile(resourcePath() + "Plane.png");
    Sprite plane(t1);

    static const int PLAN_CENTER = WIDTH - t1.getSize().x;  //비행기 이미지 크기를 뺀 x위치
    static const int PLAN_BOTTOM = HEIGHT - t1.getSize().y; //비행기 이미지 크기를 뺸 y위치

    //처음 시작할 때 비행기 위치
    Position planPos;
    planPos.x = PLAN_CENTER / 2;    //x위치 중앙에 두기 위해 나누기 2
    planPos.y = PLAN_BOTTOM;
    
    const int row = 20; //비행기가 x축에서 한 번 움직일 때마다 20씩 이동

    //총알
    Texture t2;
    t2.loadFromFile(resourcePath() + "Attack.png");
    Sprite bullet[20];      //총알의 갯수 20개
    
    int bullet_wait = 0;
    int bullet_live[20];
    int count = 0;
    for (int i = 0; i < 20; i++) {
        bullet[i].setTexture(t2);
        bullet_live[i] = 0; //0이면 비활성화, 1이면 활성화
    }

    //외계인 1
    Alien *AL1[3];
    for (int i = 0; i < 3; i++) {
        AL1[i] = new Alien;
        AL1[i]->t.loadFromFile(resourcePath() + "Alien1.png");
        AL1[i]->s.setTexture(AL1[i]->t);
        AL1[i]->setCount(3);            //3대의 총알을 맞으면 사라짐
        AL1[i]->setSpeed(rand()%4 + 1);
    }
    //외계인1의 게임 시작 전 초기 x좌표를 랜덤으로 설정
    for (int i = 0; i < 3; i++) { AL1[i]->setPos(rand() % ALIENWIDTH, 0); }
   
    //외계인 2
    Alien *AL2[3];
    for (int i = 0; i < 3; i++) {
        AL2[i] = new Alien;
        AL2[i]->t.loadFromFile(resourcePath() + "Alien2.png");
        AL2[i]->s.setTexture(AL2[i]->t);
        AL2[i]->setCount(5);            //5대의 총알을 맞으면 사라짐
        AL2[i]->setSpeed(rand()%3 + 1);
    }
    //외계인2의 게임 시작 전 초기 x좌표를 랜덤으로 설정
    for (int i = 0; i < 3; i++) { AL2[i]->setPos(rand() % ALIENWIDTH, 0); }

    //외계인 3
    Alien *AL3[3];
    for (int i = 0; i < 3; i++) {
        AL3[i] = new Alien;
        AL3[i]->t.loadFromFile(resourcePath() + "Alien3.png");
        AL3[i]->s.setTexture(AL3[i]->t);
        AL3[i]->setCount(7);            //7대의 총알을 맞으면 사라짐
        AL3[i]->setSpeed(rand()%2 + 1);
    }
    //외계인3의 게임 시작 전 초기 x좌표를 랜덤으로 설정
    for (int i = 0; i < 3; i++) { AL3[i]->setPos(rand() % ALIENWIDTH, 0); }

    //점수판
    int score = 0;
    //스코어 갱신을 화면 상에 나타내주기 위해 사용
    string scores;
    ostringstream convert;
    convert << score;
    scores = convert.str();
    
    //점수판 위치
    Position scorePos;
    scorePos.x = 820;
    scorePos.y = 10;

    //게임오버
    Font over;
    if (!over.loadFromFile(resourcePath() + "koverwatch.ttf")) {
        return EXIT_FAILURE;
    }
    Text txt("GAME OVER", over, 100);
    txt.setFillColor(Color::White);

    //게임오버 위치
    Position overPos;
    overPos.x = 335;
    overPos.y = 500;

    //게임루프 시작(화면 켜짐)
    while (window.isOpen()) {
        Event e;
        //창 닫기를 시도했을 때 창 닫기
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
        }

        //키보드
        //오른쪽 방향키 눌렀을 때 오른쪽 이동
        if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
            planPos.x += row;
        }
        //왼쪽 방향키 눌렀을 때 왼쪽 이동
        if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
            planPos.x -= row;
        }
        //스페이스 키 눌렀을 때 총알 발사
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            if (bullet_wait == 0) {
                bullet[count].setPosition(plane.getPosition()); //비행기 좌표로 총알 좌표를 맞춰줌
                bullet[count].move(46, 0);                      //총알 좌표 조정
                bullet_live[count] = 1;
                count++;
                if (count >= 20) { count = 0; }

                bullet_wait = 7;    //총알 대기 변수, 다음 총알이 나갈 때까지의 시간 = 총알의 간격
            }
        }
        
        //점수판
        Font font;
        if (!font.loadFromFile(resourcePath() + "koverwatch.ttf")) {
            return EXIT_FAILURE;
        }
        Text text;
        text.setFont(font);
        text.setString("Score : " + to_string(score));
        text.setCharacterSize(40);
        text.setFillColor(Color::White);

        //총알 동작
        for (int i = 0; i < 20; i++) {
            Vector2f bulPos = bullet[i].getPosition();
            if (bullet_live[i] == 1 && bulPos.y >= -100)
                bullet[i].move(0, -15); //총알 이동 (15)

            if (bulPos.y < -100)
                bullet_live[i] = 0;
        }
        //While문이 한 번 돌 때마다 1씩 감소하여 0이 되면 다음 총알 발사
        if (bullet_wait > 0)
            bullet_wait--;

        //외계인이 아래로 내려감
        //외계인1
        for (int i = 0; i < 3; i++) {
            AL1[i]->setSpeed(rand()%4 + 1);
            AL1[i]->s.move(0, AL1[i]->getSpeed()); }

        //외계인2
        for (int i = 0; i < 3; i++) {
            AL2[i]->setSpeed(rand()%3 + 1);
            AL2[i]->s.move(0, AL2[i]->getSpeed()); }

        //외계인3
        for (int i = 0; i < 3; i++) {
            AL3[i]->setSpeed(rand()%2 + 1);
            AL3[i]->s.move(0, AL3[i]->getSpeed()); }
 
        //###############외계인이 맨 아래 닿으면 게임오버###############
        //외계인1 오버
        for (int i = 0; i < 3; i++) {
        Vector2f al1Pos = AL1[i]->s.getPosition();
        if(al1Pos.y >= HEIGHT - 70) {
             window.draw(txt);  //게임오버
             window.display();
             sleep(30);         //30초 sleep
             exit(0);           //화면 꺼짐
             //Sleep(60000)     //window 시 sleep 1분
        }
        }
        //외계인2 오버
        for (int i = 0; i < 3; i++) {
        Vector2f al1Pos = AL2[i]->s.getPosition();
        if(al1Pos.y >= HEIGHT - 70) {
             window.draw(txt);  //게임오버
             window.display();
             sleep(30);         //30초 sleep
             exit(0);           //화면 꺼짐
             //Sleep(60000)     //window 시 sleep 1분
        }
        }
        //외계인3 오버
        for (int i = 0; i < 3; i++) {
        Vector2f al1Pos = AL3[i]->s.getPosition();
        if(al1Pos.y >= HEIGHT - 90) {
             window.draw(txt);  //게임오버
             window.display();
             sleep(30);         //30초 sleep
             exit(0);           //화면 꺼짐
             //Sleep(60000)     //window 시 sleep 1분
        }
        }

        //###############충돌###############
        //외계인1
        for (int j = 0; j < 3; j++) {
            Vector2f targetPos = AL1[j]->s.getPosition();
            for (int i = 0; i < 20; i++) {
                Vector2f bulPos2 = bullet[i].getPosition();
                float spaceX = (targetPos.x) - (bulPos2.x - 40);   //target 과 bullet의 간격 계산
                float spaceY = (targetPos.y) - (bulPos2.y);
                if (bullet_live[i] == 1 && fabs(spaceX) < 50 && fabs(spaceY) < 50) {
                    AL1[j]->setCount(AL1[j]->getCount() - 1);
                    if (AL1[j]->getCount() == 0) {
                        AL1[j]->setPos(rand() % ALIENWIDTH, 0);
                        AL1[j]->setCount(3);
                        score += 10;
                    }
                    bullet_live[i] = 0;
                }
            }
        }

        //외계인2
        for (int j = 0; j < 3; j++) {
            Vector2f targetPos = AL2[j]->s.getPosition();
            for (int i = 0; i < 20; i++) {
                Vector2f bulPos2 = bullet[i].getPosition();
                float spaceX = (targetPos.x) - (bulPos2.x - 40);   //target 과 bullet의 간격 계산
                float spaceY = (targetPos.y) - (bulPos2.y);
                if (bullet_live[i] == 1 && fabs(spaceX) < 50 && fabs(spaceY) < 50) {
                    AL2[j]->setCount(AL2[j]->getCount() - 1);
                    if (AL2[j]->getCount() == 0) {
                        AL2[j]->setPos(rand() % ALIENWIDTH, 0);
                        AL2[j]->setCount(5);
                        score += 20;
                    }
                    bullet_live[i] = 0;
                }
            }
        }
       
        //외계인3
        for (int j = 0; j < 3; j++) {
            Vector2f targetPos = AL3[j]->s.getPosition();
            for (int i = 0; i < 20; i++) {
                Vector2f bulPos2 = bullet[i].getPosition();
                float spaceX = (targetPos.x) - (bulPos2.x - 40);   //target 과 bullet의 간격 계산
                float spaceY = (targetPos.y) - (bulPos2.y);
                if (bullet_live[i] == 1 && fabs(spaceX) < 50 && fabs(spaceY) < 50) {
                    AL3[j]->setCount(AL3[j]->getCount() - 1);
                    if (AL3[j]->getCount() == 0) {
                        AL3[j]->setPos(rand() % ALIENWIDTH, 0);
                        AL3[j]->setCount(7);
                        score += 30;
                    }
                    bullet_live[i] = 0;
                }
            }
        }
      
        //비행기가 화면 밖으로 나가지 않게 설정
        //오른쪽
        if (planPos.x >= PLAN_CENTER) {
            planPos.x = PLAN_CENTER;
        }
        //왼쪽
        if (planPos.x <= PLAN_CENTER - 900) {
            planPos.x = PLAN_CENTER - 900;
        }

        //비행기 포지션
        plane.setPosition(planPos.x, planPos.y);
        //점수판 포지션
        text.setPosition(scorePos.x, scorePos.y);
        //게임오버 포지션
        txt.setPosition(overPos.x, overPos.y);
    
        window.clear(Color::Black);     //배경화면 검정
        window.draw(plane);
        window.draw(text);  //점수판
        
        //총알
        for (int i = 0; i < 20; i++) {
            if (bullet_live[i] == 1) {
                window.draw(bullet[i]);
            }
        }
        
        //외계인
        for (int i = 0; i < 3; i++) {
            window.draw(AL1[i]->s);
        }
        for (int i = 0; i < 3; i++) {
            window.draw(AL2[i]->s);
        }
        for (int i = 0; i < 3; i++) {
            window.draw(AL3[i]->s);
        }
      
        window.display();
    }

    return 0;
}
