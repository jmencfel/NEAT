#pragma once
#include "Player.h"


#include "phenotype.h"
#include "utils.h"
#include "C2DMatrix.h"
#include "SVector2D.h"
#include "CParams.h"
#include "collision.h"
#include "CMapper.h"

using namespace std;

class NEATPlayer : public Player
{
protected:
	CNeuralNet*  m_pItsBrain;

	//its memory
	CMapper     m_MemoryMap;
	//to store output from the ANN
	double			m_up, m_down, m_left, m_right, m_attack, m_defend;

	//the players fitness score. 
	double			m_dFitness;
	//if a collision has been detected this flag is set
	bool            m_bCollided;
	float delay_up, delay_down, delay_right, delay_left;
	//to store end vertices of sensor segments
	vector<SPoint>  m_Sensors;
	vector<SPoint>  m_tranSensors;

	vector<SPoint>  m_CollSensors;
	vector<SPoint>  m_transCollSensors;
	float t_idle = 0.0f;
	float f_last_fitness = 0.0f;
	//this keeps a record of how far down the sensor segment
//a 'hit' has occurred.
	vector<double>  m_vecdSensors;
	vector<bool>  m_vecCollSensors;

	//the end points of the sensors check their coordinate
	//cell to see how many times the sweeper has visited it.
	vector<double>  m_vecFeelers;
	void      CreateSensors(vector<SPoint> &sensors,
		int            NumSensors,
		double         range);
	void      CreateColSensors(vector<SPoint> &sensors,
		int            NumSensors,
		double         range);


	int       CheckForHit(vector<SVector2D> &objects, double size);

	void      TestSensors();

	void      TestRange();
	
	

public:

	NEATPlayer();
	void Reset();
	
	void      EndOfRunCalculations();
	void setStart(sf::Vector2f pos) { start_positon = pos; setPosition(pos); }
	void      RenderStats(HDC surface);
	bool isPlayer() override;
	void      DrawNet(sf::RenderWindow &window, int cxLeft, int cxRight, int cyTop, int cyBot)
	{
		m_pItsBrain->DrawNet(window, cxLeft, cxRight, cyTop, cyBot);
	}
	void setBest();
	void setNormal();
	
	//float getFitness() { return time_alive + (kills*50.0f) - damage_taken + damage_done; };
	float getFitness() { return damage_done / 100.0f  + distance_walked/10000.0f; }
	void Die() override;
	void Update(const float &dt) override;
	bool NN_Update(const float &dt);
	void			WorldTransform(vector<SPoint> &sweeper, double scale);

	float			        Fitness()const { return m_dFitness; }


	vector<SPoint>&   Sensors() { return m_tranSensors; }
	vector<SPoint>&   ColSensors() { return m_transCollSensors; }

	vector<double>&   SensorReadings() { return m_vecdSensors; }
	vector<bool>&   ColSensorReadings() { return m_vecCollSensors; }


	vector<double>    MemoryReadings() { return m_vecFeelers; }

	int               NumCellsVisited() { return m_MemoryMap.NumCellsVisited(); }

	void              InsertNewBrain(CNeuralNet* brain) { m_pItsBrain = brain; }
};