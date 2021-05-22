#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

//포지션 구조체
struct Position {
   int x;
   int y;
};

//외계인 클래스 함수
class Alien {
private:
   int count = 0;
   int speed = 0;
   Position Pos;
public:
   Texture t;
   Sprite s;

   void setPos(int a, int b);
   void setCount(int a);
   void setSpeed(int a);

   int getCount();
   int getSpeed();
};
