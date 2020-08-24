#include "Invader.h"
#include "Game.h"
#include "maths.h"
#include <iostream>
#include "CParams.h"
using namespace sf;


bool Invader::direction = true;
float Invader::speed = 50.0f;

Invader::Invader() : Player() {};

Invader::Invader(Vector2f pos)
{
	alive = true;
	setOrigin(32, 32);
	setPosition(pos);
	start_positon = pos;
}
Invader::Invader(Vector2f pos, Player* tar)
{
	alive = true;
	setOrigin(32, 32);
	setPosition(pos);
	target = tar;
	start_positon = pos;
}

void Invader::Update(const float &dt)
{
	Player::Update(dt);
	if (t_idle > 30.0f)
		Die();
	if (alive)
	{
		hit_landed = false;
		fireTime -= dt ;
		t_idle += dt ;
		isMoving = false;

		if (miss_count > 0)
		{
			if (facing == 1)
			{
				facing = 2;
				face_x = 1.0f;
				face_y = 0.0f;
				miss_count = 0;
				
			}
			else if (facing == 2)
			{
				facing = 3;
				face_x = 0.0f;
				face_y = 1.0f;
				miss_count = 0;
			}
			else if (facing == 3)
			{
				facing = 4;
				face_x = -1.0f;
				face_y = 0.0f;
				miss_count = 0;
			}
			else if (facing == 4)
			{
				facing = 1;
				face_x = 0.0f;
				face_y = -1.0f;
				miss_count = 0;
			}
		}
		Vector2f vec = (target->getPosition() - target->getOrigin()) - (getPosition() - getOrigin());
		if(!pushed)
		if (length((target->getPosition() - target->getOrigin()) - (getPosition() - getOrigin())) >= 32.0f)
		{
			isMoving = true;
			if (vec.x >= 0 && vec.y >= 0)
			{
				if (vec.x > vec.y)
				{
					facing = 2;
					face_x = 1.0f;
					face_y = 0.0f;
				}
				else
				{
					facing = 3;
					face_x = 0.0f;
					face_y = 1.0f;
				}
			}
			else if (vec.x >= 0 && vec.y < 0)
			{
				if (abs(vec.x) > abs(vec.y))
				{
					facing = 2;
					face_x = 1.0f;
					face_y = 0.0f;
				}
				else
				{
					facing = 1;
					face_x = 0.0f;
					face_y = -1.0f;
				}
			}
			else if (vec.x < 0 && vec.y < 0)
			{
				if (abs(vec.x) > abs(vec.y))
				{
					facing = 4;
					face_x = -1.0f;
					face_y = 0.0f;
				}
				else
				{
					facing = 1;
					face_x = 0.0f;
					face_y = -1.0f;
				}
			}
			else if (vec.x < 0 && vec.y >= 0)
			{
				if (abs(vec.x) > abs(vec.y))
				{
					facing = 4;
					face_x = -1.0f;
					face_y = 0.0f;
				}
				else
				{
					facing = 3;
					face_x = 0.0f;
					face_y = 1.0f;
				}
			}
			if (validMove(getPosition() + 100.0f*dt*normalize(target->getPosition() - getPosition())))
			{
				if(target->isAlive())
				move(100.0f*dt *normalize(target->getPosition() - getPosition()));
			}
		}
		else
		{
			if (fireTime <= 0.0f)
			{
				for (auto p : m_vecPlayers)
				{
					if(p->isAlive())
					if (length((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()) - Vector2f(32.0f*face_x, 32.0f*face_y)) <= 50.0f)
					{
						if (!p->isDefending())
						{
							//std::cerr << "HIT!" << std::endl;
							Vector2f push = normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
							if(validMove(p->getPosition() + Vector2f(push.x, push.y)))
							{ 
								p->Push(push.x, push.y);
							}
							else
							{
								p->Push(0, 0);
							}
							t_idle = 0;
							p->getHit(20.0f);
							miss_count = 0;
							hit_landed = true;
						}
						else
						{
							Vector2f push = 0.5f *normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
							if (validMove(getPosition() - Vector2f(push.x, push.y)))
							{
								Push(-push.x, -push.y, false);
							}
							else
							{
								Push(0, 0, false);
							}
							if (validMove(p->getPosition() + Vector2f(push.x, push.y)))
							{
								p->Push(push.x, push.y, false);
							}
							else
							{
								Push(0, 0, false);
							}
							t_idle = 0;
							miss_count = 0;
							hit_landed = true;
							p->parry();
						}

					}
				}
				isAttacking = true;
				fireTime = 1.0f;
				if (!hit_landed)
					miss_count++;
			}
		}
	}
	
	
	
		
	
	
}


