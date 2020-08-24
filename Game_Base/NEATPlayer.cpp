#include "NEATPlayer.h"
#include "Game.h"
#include <iostream>
#include "maths.h"
#include "CParams.h"

using namespace sf;
int chosen_output_id;
NEATPlayer::NEATPlayer() : m_dFitness(0), m_up(0), m_down(0), m_right(0), m_left(0), m_attack(0), m_defend(0)
{
	alive = true;
	show_hud = true;
	isNEAT = true;
	setTexture(player_spritesheet);
	setTextureRect({ 168, 23, 23, 26 });
	
	CreateSensors(m_Sensors, CParams::iNumSensors, CParams::dSensorRange);
	CreateColSensors(m_CollSensors, CParams::iNumColSensors, CParams::dColSensorRange);
	m_MemoryMap.Init(CParams::WindowWidth,
		CParams::WindowHeight);
}

void NEATPlayer::CreateSensors(vector<SPoint> &sensors,
	int            NumSensors,
	double         range)
{
	//make sure vector of sensors is empty before proceeding
	sensors.clear();

	double SegmentAngle = 2*CParams::dPi / (NumSensors);

	//going clockwise from 90deg left of position calculate the fan of
	//points radiating out (not including the origin)
	for (int i = 0; i < CParams::iNumSensors; i++)
	{
		//calculate vertex position
		SPoint point;

		point.x = -sin(i * SegmentAngle - CParams::dHalfPi) * range;

		point.y = cos(i * SegmentAngle - CParams::dHalfPi) * range;

		sensors.push_back(point);
		
	}//next segment
	
}
void NEATPlayer::CreateColSensors(vector<SPoint> &sensors,
	int            NumSensors,
	double         range)
{
	//make sure vector of sensors is empty before proceeding
	sensors.clear();

	double SegmentAngle = 2 * CParams::dPi / (NumSensors);

	//going clockwise from 90deg left of position calculate the fan of
	//points radiating out (not including the origin)
	for (int i = 0; i < CParams::iNumColSensors; i++)
	{
		//calculate vertex position
		SPoint point;

		point.x = -sin(i * SegmentAngle - CParams::dHalfPi) * range;

		point.y = cos(i * SegmentAngle - CParams::dHalfPi) * range;

		sensors.push_back(point);

	}//next segment

}
bool NEATPlayer::isPlayer()
{
	return true;
}
void NEATPlayer::Die()
{
	//Reset();
	
	alive = false;
	setColor(Color::Black);
	m_MemoryMap.Reset();

}
void NEATPlayer::Reset()
{
	//reset the sweepers positions
	setColor(default_color);
	//and the fitness
	setPosition(start_positon);
	m_dFitness = 0;
	health = 100;
	t_idle = 0.0f;
	damage_taken = 0;
	damage_done = 0;
	distance_walked = 0.0f;
	alive = true;
	kills = 0;
	time_alive = 0;

	//reset its memory
	m_MemoryMap.Reset();

}
void NEATPlayer::setNormal()
{
	is_best = false;
	default_color = sf::Color::White;
}
void NEATPlayer::setBest()
{
	is_best = true;
	default_color = sf::Color::Magenta;
}
bool NEATPlayer::NN_Update(const float &dt)
{
	//this will store all the inputs for the NN
	if (alive)
	{
		vector<double> inputs;
		t_idle += dt;
		if (t_idle > 25.0f)
			Die();
		//grab sensor readings
		TestSensors();
		for (int sr = 0; sr < m_vecdSensors.size(); ++sr)
		{
			inputs.push_back(m_vecdSensors[sr]);
			//	inputs.push_back(m_vecFeelers[sr]);

		}
		for (int sr = 0; sr < m_vecCollSensors.size(); ++sr)
		{
			inputs.push_back(m_vecCollSensors[sr]);

		}
		inputs.push_back((2.0f*health / max_health) - 1.0f);
		inputs.push_back((facing / 4.0f));



		//update the brain and get feedback
		vector<double> output = m_pItsBrain->Update(inputs, CNeuralNet::active);

		//make sure there were no errors in calculating the 
		//output
		if (output.size() < CParams::iNumOutputs)
		{
			return false;
		}
		chosen_output_id = -1;
		double min_o = -100.0f;
		for (int i = 0; i < output.size(); i++)
		{
			if (output[i] > min_o)
			{
				chosen_output_id = i;
				min_o = output[i];
			}
		}
	}
	Update(dt);
	m_MemoryMap.Update(getPosition().x, getPosition().y);
	return true;
}
void NEATPlayer::Update(const float &dt)
{
	if (CParams::bRender)
	if (show_hud && is_best)
	{
		text_last_fitness.setPosition(getPosition() - getOrigin() + Vector2f(0, -60.0f));
		text_last_fitness.setString("Fitness:" + std::to_string(f_last_fitness));

	}
	if (alive)
	{
		Player::Update(dt);
		//movement
		fireTime -= dt ;
		defendDelay -= dt;
		delay_down -= dt;
		delay_up -= dt;
		delay_right -= dt;
		delay_left -= dt;
		float direction = 0.0f;
		isMoving = false;
		defending = false;
	
		if (!pushed)
		{
			if (chosen_output_id == 3)
			{
				if (validMove(getPosition() + Vector2f(100 * dt, 0)))
				{
					move(100 * dt, 0);
					if (delay_right <= 0)
						distance_walked += 100 * dt;
				}
				delay_left = 0.5f;
				delay_up = 0.5f;
				delay_down = 0.5f;
				facing = 2;
				face_x = 1.0f;
				face_y = 0.0f;
				isMoving = true;
			}
			if (chosen_output_id == 2)
			{
				if (validMove(getPosition() + Vector2f(-100 * dt, 0)))
				{
					move(-100 * dt, 0);
					if (delay_left <= 0)
						distance_walked += 100 * dt;
				}
				delay_right = 0.5f;
				delay_up = 0.5f;
				delay_down = 0.5f;
				facing = 4;
				face_x = -1.0f;
				face_y = 0.0f;
				isMoving = true;
			}
			if (chosen_output_id == 0)
			{
				if (validMove(getPosition() + Vector2f(0, -100 * dt)))
				{
					move(0, -100 * dt);
					if (delay_up <= 0)
						distance_walked += 100 * dt;
				}
				delay_left = 0.5f;
				delay_right = 0.5f;
				delay_down = 0.5f;
				facing = 1;
				face_x = 0.0f;
				face_y = -1.0f;
				isMoving = true;
			}
			if (chosen_output_id == 1)
			{
				if (validMove(getPosition() + Vector2f(0, 100 * dt)))
				{
					move(0, 100 * dt);
					if (delay_down <= 0)
						distance_walked += 100 * dt;
				}
				delay_left = 0.5f;
				delay_up = 0.5f;
				delay_right = 0.5f;
				facing = 3;
				face_x = 0.0f;
				face_y = 1.0f;
				isMoving = true;
			}

			if (chosen_output_id == 5)
			{

				if (defendDelay <= 0)
					defending = true;
			}
			if (chosen_output_id == 4)
				if (fireTime <= 0.0f)
				{

					for (auto p : m_vecEnemies)
					{
						if (p == this || !p->isAlive())
							continue;
						if (length((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()) - Vector2f(32.0f*face_x, 32.0f*face_y)) <= 32.0f)
						{
							if (!p->isDefending())
							{
								Vector2f push = normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
								p->Push(push.x, push.y);
								p->getHit(20.0f);
								idle_time = 0.0f;
								damage_done += 20.0f;
								t_idle = 0.0f;
								if (p->getHealth() <= 0)
								{
									addKill();
									setPosition(start_positon);
									//health += 50;
									//if (health > 100) health = 100;
								}
							}
							else
							{
								Vector2f push = 0.5f*normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
								Push(-push.x, -push.y, false);
								p->Push(push.x, push.y, false);
								t_idle = 0.0f;
							}

						}
					}
					isAttacking = true;
					fireTime = 0.5f;

				}
		}
		
	}
	
	
}


void NEATPlayer::EndOfRunCalculations()
{
	//m_dFitness += time_alive + (kills*50.0f) - damage_taken + damage_done;
	m_dFitness += damage_done / 100.0f + distance_walked / 10000.0f;
	f_last_fitness = m_dFitness;
}
void NEATPlayer::WorldTransform(vector<SPoint> &sweeper, double scale)
{
	//create the world transformation matrix
	C2DMatrix matTransform;

	//scale
	matTransform.Scale(scale, scale);

	//rotate
	//matTransform.Rotate(m_dRotation);

	//and translate
	matTransform.Translate(getPosition().x, getPosition().y);

	//now transform the ships vertices
	matTransform.TransformSPoints(sweeper);
}
void NEATPlayer::TestSensors()
{
	m_bCollided = false;


	m_tranSensors = m_Sensors;

	m_transCollSensors = m_CollSensors;

	WorldTransform(m_tranSensors, 1);  //scale is 1
	WorldTransform(m_transCollSensors, 1);  //scale is 1
	//flush the sensors
	m_vecdSensors.clear();
	m_vecCollSensors.clear();
	m_vecFeelers.clear();

	//now to check each sensor against the objects in the world
	for (int sr = 0; sr < m_tranSensors.size(); ++sr)
	{
		bool bHit = false;

		double dist = 0;

		for (int seg = 0; seg < m_vecEnemies.size(); seg++)
		{
			if (LineIntersection2D(SPoint(getPosition().x, getPosition().y),
				SPoint(+ m_tranSensors[sr].x,m_tranSensors[sr].y),
				SPoint(m_vecEnemies[seg]->getPosition().x- 16.0f, m_vecEnemies[seg]->getPosition().y+16.0f ),
				SPoint(m_vecEnemies[seg]->getPosition().x + 16.0f, m_vecEnemies[seg]->getPosition().y + 16.0f),
				dist) 
				||
				LineIntersection2D(SPoint(getPosition().x, getPosition().y),
				SPoint(+m_tranSensors[sr].x, m_tranSensors[sr].y),
				SPoint(m_vecEnemies[seg]->getPosition().x - 16.0f, m_vecEnemies[seg]->getPosition().y - 16.0f),
				SPoint(m_vecEnemies[seg]->getPosition().x + 16.0f, m_vecEnemies[seg]->getPosition().y + 16.0f),
				dist)
				||
				LineIntersection2D(SPoint(getPosition().x, getPosition().y),
				SPoint(m_tranSensors[sr].x,m_tranSensors[sr].y),
				SPoint(m_vecEnemies[seg]->getPosition().x + 16.0f , m_vecEnemies[seg]->getPosition().y  - 16.0f),
				SPoint(m_vecEnemies[seg]->getPosition().x + 16.0f, m_vecEnemies[seg]->getPosition().y  + 16.0f),
				dist)
				||
				LineIntersection2D(SPoint(getPosition().x, getPosition().y),
				SPoint(m_tranSensors[sr].x, m_tranSensors[sr].y),
				SPoint(m_vecEnemies[seg]->getPosition().x - 16.0f, m_vecEnemies[seg]->getPosition().y - 16.0f),
				SPoint(m_vecEnemies[seg]->getPosition().x - 16.0f, m_vecEnemies[seg]->getPosition().y + 16.0f),
				dist)
				)
			
			{

				bHit = true;
				break;
			}
		}

		if (bHit)
		{
			m_vecdSensors.push_back(dist);

			//implement very simple collision detection
			//if (dist < CParams::dCollisionDist)
			{
				
			}
		}
		else
		{
			m_vecdSensors.push_back(-1);
		}

		//check how many times the minesweeper has visited the cell
		//at the current position
	
		int HowOften = m_MemoryMap.TicksLingered(m_tranSensors[sr].x,
			m_tranSensors[sr].y);


		//Update the memory info according to HowOften. The maximum
		//value is 1 (because we want all the inputs into the
		//ANN to be scaled between -1 < n < 1)
		if (HowOften == 0)
		{
			m_vecFeelers.push_back(-1);

			continue;
		}

		if (HowOften < 10)
		{
			m_vecFeelers.push_back(0);

			continue;
		}

		if (HowOften < 20)
		{
			m_vecFeelers.push_back(0.2);

			continue;
		}

		if (HowOften < 30)
		{
			m_vecFeelers.push_back(0.4);

			continue;
		}

		if (HowOften < 50)
		{
			m_vecFeelers.push_back(0.6);

			continue;
		}

		if (HowOften < 80)
		{
			m_vecFeelers.push_back(0.8);

			continue;
		}

		m_vecFeelers.push_back(1);
		for (int sr = 0; sr < m_tranSensors.size(); ++sr)
		{

		}
	}//next sensor
	for (int sr = 0; sr < m_transCollSensors.size(); ++sr)
	{
		if (validMove(sf::Vector2f(m_transCollSensors[sr].x, m_transCollSensors[sr].y)))
		{
			m_vecCollSensors.push_back(true);
			m_bCollided = true;
		}
		else
		{ 
			m_vecCollSensors.push_back(false);
		}
	}
	
	
}
