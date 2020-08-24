#include "HumanPlayer.h"
#include "Game.h"
#include <iostream>
#include "maths.h"

using namespace sf;
HumanPlayer::HumanPlayer()
{
	alive = true;
	show_hud = true;
	setTexture(player_spritesheet);
	setTextureRect({ 168, 23, 23, 26 });
	setPosition({gameWidth/2, gameHeight/2});
}
bool HumanPlayer::isPlayer()
{
	return true;
}
void HumanPlayer::Update(const float &dt)
{
	if (alive)
	{
		Player::Update(dt);
		//movement
		fireTime -= dt;
		float direction = 0.0f;
		isMoving = false;
		defending = false;
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			move(100 * dt, 0);
			facing = 2;
			face_x = 1.0f;
			face_y = 0.0f;
			isMoving = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			move(-100 * dt, 0);
			facing = 4;
			face_x = -1.0f;
			face_y = 0.0f;
			isMoving = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			move(0, -100 * dt);
			facing = 1;
			face_x = 0.0f;
			face_y = -1.0f;
			isMoving = true;
		}
		 if (Keyboard::isKeyPressed(Keyboard::S))
		{
			move(0, 100 * dt);
			facing = 3;
			face_x = 0.0f;
			face_y = 1.0f;
			isMoving = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::C))
		 {
			defending = true;
		}
		if (fireTime <= 0.0f && Keyboard::isKeyPressed(Keyboard::Space))
		{
			
			for (auto p : m_vecEnemies)
			{
				if (p == this)
					continue;
				if (length((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin())- Vector2f(32.0f*face_x, 32.0f*face_y)) <= 50.0f)
				{
					if (!p->isDefending())
					{
						std::cerr << "HIT!" << std::endl;
						Vector2f push = normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
						p->Push(push.x, push.y);
						p->getHit(20.0f);
						if (p->getHealth() <= 0)
							addKill();
					}
					else
					{
						Vector2f push = 0.5f*normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
						Push(-push.x, -push.y, false);
						p->Push(push.x, push.y, false);
					}
					
				}
			}
			isAttacking = true;
			fireTime = 0.5f;

		}
	}
	else
	{
		destructionDelay -= dt;
	}
	if (destructionDelay <= 0.0f)
	{
		setPosition({ -100,-100 });
		std::cout << "GAME OVER!" << std::endl;
	}
}
