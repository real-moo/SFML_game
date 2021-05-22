//
//  Alien.cpp
//  Game
//
//  Created by 김민주 on 2020/06/26.
//  Copyright © 2020 김민주. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Alien.hpp"

using namespace std;
using namespace sf;



void Alien::setPos(int a, int b) {
   Pos.x = a;
   Pos.y = b;
   s.setPosition(Pos.x, Pos.y);
}
void Alien::setCount(int a) {
   count = a;
}
void Alien::setSpeed(int a) {
   speed = a;
}

int Alien::getCount() {
   return count;
}
int Alien::getSpeed() {
   return speed;
}
