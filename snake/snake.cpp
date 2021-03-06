﻿// snake.cpp : Defines the entry point for the console application.
//

/*
CHANGES DUE TUESDAY

1: change image of food to another 16*16 pixel
2: increment the speed/time every time snake eats something
3: add 2nd player -->create another snake controlled w WASD
-->if 2nd eats first follow snake rules to fix
*/

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

//globals
int num_vertBox = 30, num_horiBox = 20;
int size = 16;
int w = size * num_horiBox; //bg num pixels width
int l = size * num_vertBox; //bg num pixels height

struct snake
{
	int x, y;
	int len = 0;
	float delay = .1;
} s1[100]; //max size 100
snake s2[100];


struct fruit
{
	int x, y;
}food; //one sprite

void tick(snake s[], int dir)
{
	//move remaining pieces of snake s[1]-s[99]
	for (int i = s[0].len; i > 0; --i)
	{
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}

	if (dir == 3) //up
	{
		s[0].y -= 1;
	}
	if (dir == 0) //down
	{
		s[0].y += 1;
	}
	if (dir == 1) //left
	{
		s[0].x -= 1;
	}
	if (dir == 2) //right
	{
		s[0].x += 1;
	}
	if (s[0].x == food.x && s[0].y == food.y)
	{
		s[0].len++;
		food.x = rand() % num_horiBox;
		food.y = rand() % num_vertBox;
		s[0].delay -= .01;
	}

	//boundary check for LRUD
	if (s[0].y > num_vertBox)
		s[0].y = 0;
	if (s[0].y < 0)
		s[0].y = num_vertBox;
	if (s[0].x > num_horiBox)
		s[0].x = 0;
	if (s[0].x < 0)
		s[0].x = num_horiBox;

	for (int i = 1; i < s[0].len; i++)
	{
		if (s[0].x == s[i].x && s[0].y == s[i].y)
		{
			s[0].len = i;
			s[0].delay = .01*i;
		}

	}
}

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(w, l), "Snake");

	Texture t1, t2, t3, t4;
	t1.loadFromFile("image/white.png");
	t2.loadFromFile("image/snake1.png");
	t3.loadFromFile("image/fruit.png");
	t4.loadFromFile("image/snake2.png");
	Sprite spr1(t1);
	Sprite spr2(t2);
	Sprite spr3(t3);
	Sprite spr4(t4);

	food.x = 10;
	food.y = 10;
	s1[0].x = 0;
	s1[0].y = 0;
	s2[0].x = 19;
	s2[0].y = 29;
	s1[0].delay = 0.1;
	s2[0].delay = 0.1;

	Clock clock;
	float timer1 = 0, timer2 = 0;
	bool initial = true;
	s1[0].len = 4; //setting initial snake length
	s2[0].len = 4;
	int dir1 = 0;
	int dir2 = 0;
	while (window.isOpen())
	{
		Event e;

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer1 += time;
		timer2 += time;

		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Up)) dir1 = 3;
		if (Keyboard::isKeyPressed(Keyboard::Down)) dir1 = 0;
		if (Keyboard::isKeyPressed(Keyboard::Left)) dir1 = 1;
		if (Keyboard::isKeyPressed(Keyboard::Right)) dir1 = 2;
		
		if (Keyboard::isKeyPressed(Keyboard::W)) dir2 = 3;
		if (Keyboard::isKeyPressed(Keyboard::S)) dir2 = 0;
		if (Keyboard::isKeyPressed(Keyboard::A)) dir2 = 1;
		if (Keyboard::isKeyPressed(Keyboard::D)) dir2 = 2;
		
		if (timer1 > s1[0].delay)
		{
			timer1 = 0;
			tick(s1, dir1); //move snake 1 forward
			if (initial==false)
			{
				for (int i = 0; i < s2[0].len; i++)
				{
					if (s1[0].x == s2[i].x && s1[0].y == s2[i].y)
					{
						s2[0].len = i;
						s1[0].len += i;
						s2[0].delay = .01*i;
					}
				}
			}
		}
		if (timer2 > s2[0].delay)
		{
			timer2 = 0;
			tick(s2, dir2); //move snake 2 forward
			if (initial == false)
			{
				for (int i = 0; i < s1[0].len; i++)
				{
					if (s1[i].x == s2[0].x && s1[i].y == s2[0].y)
					{
						s1[0].len = i;
						s2[0].len += i;
						s1[0].delay = .01*i;
					}
				}
			}
		}

		window.clear();

		for (int i = 0; i < num_horiBox; i++)
		{
			for (int j = 0; j < num_vertBox; j++)
			{
				spr1.setPosition(i*size, j*size);
				window.draw(spr1);
			}
		}

		for (int i = 0; i < s1[0].len; i++)
		{
			spr2.setPosition(s1[i].x*size, s1[i].y*size);
			window.draw(spr2);
		}

		for (int i = 0; i < s2[0].len; i++)
		{
			spr4.setPosition(s2[i].x*size, s2[i].y*size);
			window.draw(spr4);
		}

		spr3.setPosition(food.x*size, food.y*size);
		window.draw(spr3);

		window.display();
		if(initial==true)
			initial = false;
	}

	return 0;
}
