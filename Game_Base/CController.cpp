#include "CController.h"
#include <ctime>



std::vector<NEATPlayer*> m_vecPlayers;
std::vector<Player*> m_vecEnemies;
NEATPlayer* pointer_bestPlayer;
int spawn_cycle = 0;
std::vector<sf::Vector2f> new_spawns;
bool show_sensors = false;
float show_sensors_delay = 0.0f;
float avg_fit = 0.0f;
float best_fitness = 0.0f;
ofstream data_output;
//these hold the geometry of the sweepers and the mines

//---------------------------------------constructor---------------------
//
//	initilaize the sweepers, their brains and the GA factory
//
//-----------------------------------------------------------------------
CController::CController(int  cxClient,
                         int  cyClient): m_NumSweepers(CParams::iPopSize), 
										                     m_bFastRender(false),
                                         m_bRenderBest(false),
										                     m_iTicks(0.0f),										                     
                                         m_hwndInfo(NULL),
											m_iGenerations(0),
                                         m_cxClient(cxClient),
                                         m_cyClient(cyClient),
                                         m_iViewThisSweeper(0),
										bExperimentComplete(false)
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	char buffer[80];
	std::strftime(buffer, 80, "fitness/%F--%H-%M-%S .csv",  now);
	data_output.open(buffer);

	//let's create the mine sweepers
	auto spawns = ls::findTiles(ls::START);
	spawns_mult.push_back(ls::findTiles(ls::ENEMY0));
	spawns_mult.push_back(ls::findTiles(ls::ENEMY1));
	spawns_mult.push_back(ls::findTiles(ls::ENEMY2));
	spawns_mult.push_back(ls::findTiles(ls::ENEMY3));

	gen.setFont(font);
	gen.setCharacterSize(30.0f);
	
	gen.setColor(sf::Color::Red);
	for (int i = 0; i < m_NumSweepers; ++i)
	{
		NEATPlayer* p1 = new NEATPlayer();
		p1->setStart(spawns[i]);
		m_vecPlayers.push_back(p1);

		Invader* inv = new Invader(spawns_mult[0][i], m_vecPlayers[i]);
		m_vecEnemies.push_back(inv);

	}
	
  
  //and the vector of sweepers which will hold the best performing sweeprs
  for (int i=0; i<CParams::iNumBestSweepers; ++i)
	{
		NEATPlayer* p1 = new NEATPlayer();
		m_vecBestPlayers.push_back(p1);
	}

  
  if(CParams::bLoad)
  { 
	  
		m_pPop = new Cga(CParams::iPopSize,
                   CParams::iNumInputs,
                   CParams::iNumOutputs, true);
  }
  else 
  {
	  m_pPop = new Cga(CParams::iPopSize,
				  	   CParams::iNumInputs,
					   CParams::iNumOutputs);
  }

  //create the phenotypes
   vector<CNeuralNet*> pBrains = m_pPop->CreatePhenotypes();
   
	//assign the phenotypes
  for (int i=0; i<m_NumSweepers; i++)
  {   
    m_vecPlayers[i]->InsertNewBrain(pBrains[i]);
  }

	
	
}

//--------------------------------------destructor-------------------------------------
//
//--------------------------------------------------------------------------------------
CController::~CController()
{
	data_output.close();
  if (m_pPop)
  {
    delete m_pPop;
  }
  for (auto m : m_vecPlayers)
  {
	  m->Die();
  }
  m_vecPlayers.clear();
  for (auto m : m_vecEnemies)
  {
	  m->Die();
  }
  m_vecEnemies.clear();
  for (auto m : m_vecBestPlayers)
  {
	  m->Die();
  }
  m_vecBestPlayers.clear();
}

sf::Vector2f CController::pickSpawn(int id, int cycle)
{
	spawn_cycle++;
	return spawns_mult[cycle%4][id];
}
//-------------------------------------Update---------------------------------------
//
//	This is the main workhorse. The entire simulation is controlled from here.
//
//--------------------------------------------------------------------------------------
bool CController::Update(const float &dt)
{
	//run the sweepers through NUM_TICKS amount of cycles. During this loop each
	//sweepers NN is constantly updated with the appropriate information from its 
	//surroundings. The output from the NN is obtained and the sweeper is moved.

	if (m_iGenerations > CParams::iNumGenerations)
	{
		bExperimentComplete = true;
	}
	m_iTicks += dt/CParams::fSpeedUp;
	if (!lock_keys && show_sensors_delay <= 0 && Keyboard::isKeyPressed(Keyboard::U))
	{
		for (auto p : m_vecPlayers)
		{
			if (p->isAlive())
			{
				cam_pos = p->getPosition();
				break;
			}
		}
		show_sensors_delay = 0.3f;
	}
	if (!lock_keys && show_sensors_delay <= 0 && Keyboard::isKeyPressed(Keyboard::Y))
	{
		for (auto p : m_vecPlayers)
		{
			if (p->isBest())
			{
				cam_pos = p->getPosition();
				break;
			}
		}
		show_sensors_delay = 0.3f;
	}
	show_sensors_delay -= dt;
	if (!lock_keys && show_sensors_delay <= 0 && Keyboard::isKeyPressed(Keyboard::B))
	{
		show_sensors = (show_sensors) ? false : true;
		show_sensors_delay = 0.3f;
	}
	int left_alive = m_vecPlayers.size();



	if (m_iGenerations < 1)
	{
		gen.setString("Generation: " + std::to_string(m_iGenerations) + "\nNum Species: " + itos(m_pPop->NumSpecies()) + "\nAverage Fitness: " + std::to_string(avg_fit) + "\nBest Fitness so far: " + ftos(m_pPop->BestEverFitness()) + "\nBest fitness this gen:" + std::to_string(best_fitness) + "\nFPS:" + std::to_string((int)(1.0f*CParams::fSpeedUp / dt)));
	}
	else
	{
		std::string temp = m_pPop->SpeciesInfo();
		gen.setString("Generation: " + std::to_string(m_iGenerations) + "\nNum Species: " + itos(m_pPop->NumSpecies()) + "\nAverage Fitness: " + std::to_string(avg_fit) + "\nAll time Best Fitness so far: " + ftos(m_pPop->BestEverFitness()) + "\nBest fitness this gen:" + std::to_string(best_fitness) + temp + "\nFPS:" + std::to_string((int)(1.0f*CParams::fSpeedUp / dt)));
	}
	for (auto p : m_vecPlayers)
	{
		if (!p->isAlive())
		{
			left_alive--;
		}
	}

	if (m_iTicks < CParams::fTimeLimit  && left_alive > 0)
	{
		for (int i = 0; i < m_NumSweepers; ++i)
		{
			if (!m_vecEnemies[i]->isAlive())
			{
				m_vecEnemies[i]->Reset(pickSpawn(i, spawn_cycle));
			}
		}
		for (int i=0; i<m_NumSweepers; ++i)
		{
			//update the NN and position
			if (!m_vecPlayers[i]->NN_Update(dt))
			{
				//error in processing the neural net
				MessageBox(m_hwndMain, "Wrong amount of NN inputs!", "Error", MB_OK);

				return false;
			}
		}

    //update the NNs of the last generations best performers
    if (m_iGenerations > 0)
    {
      for (int i=0; i<m_vecBestPlayers.size(); ++i)
      {
         //update the NN and position
			   if (!m_vecBestPlayers[i]->NN_Update(dt))
			   {
				    //error in processing the neural net
				    MessageBox(m_hwndMain, "Wrong amount of NN inputs!", "Error", MB_OK);

				    return false;
			   }
      }
    }
	}

	//We have completed another generation so now we need to run the GA
	else if (m_iTicks >= CParams::fTimeLimit || left_alive <=0 )
	{    
		
    //first add up each sweepers fitness scores. (remember for this task
    //there are many different sorts of penalties the sweepers may incur
    //and each one has a coefficient)
	avg_fit = 0.0f;
	best_fitness = -100.0f;
    for (int swp=0; swp< m_vecPlayers.size(); ++swp)
    {

		m_vecPlayers[swp]->setNormal();
		float curr_fitness = m_vecPlayers[swp]->getFitness();
		m_vecPlayers[swp]->EndOfRunCalculations();
		avg_fit += curr_fitness;
		if (curr_fitness > best_fitness)
		{
			best_fitness = curr_fitness;
			pointer_bestPlayer = m_vecPlayers[swp];
		}
    }
	pointer_bestPlayer->setBest();
	avg_fit /= m_vecPlayers.size();
	data_output << std::to_string(m_iGenerations) << ", " <<std::to_string(avg_fit) << ","<< ftos(m_pPop->BestEverFitness()) <<","<< std::to_string(best_fitness)<<  endl;
		//increment the generation counter
		++m_iGenerations;

		//reset cycles
		m_iTicks = 0;

    //perform an epoch and grab the new brains
    vector<CNeuralNet*> pBrains = m_pPop->Epoch(GetFitnessScores());
			
		//insert the new  brains back into the sweepers and reset their
    //positions

	
	spawn_cycle++;

    for (int i=0; i<m_NumSweepers; ++i)
		{
		m_vecPlayers[i]->InsertNewBrain(pBrains[i]);

		

		m_vecEnemies[i]->Reset();
		m_vecEnemies[i]->setPosition(pickSpawn(i, spawn_cycle));
		m_vecPlayers[i]->Reset();
		
		}
	
    //grab the NNs of the best performers from the last generation
    vector<CNeuralNet*> pBestBrains = m_pPop->GetBestPhenotypesFromLastGeneration();

    //put them into our record of the best sweepers

    for (int i=0; i< m_vecBestPlayers.size(); ++i)
		{
		m_vecBestPlayers[i]->InsertNewBrain(pBestBrains[i]);
		
		m_vecBestPlayers[i]->Reset();
		}

        //this will call WM_PAINT which will render our scene
	//  InvalidateRect(m_hwndInfo, NULL, TRUE);
	//	UpdateWindow(m_hwndInfo);

	}
	gen.setPosition(cam_pos.x - gameWidth/2.05f , cam_pos.y- gameHeight/2.05f);
	return true;
}

//---------------------------------- RenderNetworks ----------------------
//
//  Renders the best four phenotypes from the previous generation
//------------------------------------------------------------------------
void CController::RenderNetworks(RenderWindow &window)
{
  if (m_iGenerations < 1)
  {
    return;
  }

  //draw the network of the best 4 genomes. First get the dimensions of the 
   //info window
#

	int	cxInfo = gameWidth;
	int	cyInfo = gameHeight;

   //now draw the 4 best networks
	m_vecBestPlayers[0]->DrawNet(window, 0, cxInfo/2, cyInfo/2, 0);
	m_vecBestPlayers[1]->DrawNet(window, cxInfo/2, cxInfo, cyInfo/2, 0);
	m_vecBestPlayers[2]->DrawNet(window, 0, cxInfo/2, cyInfo, cyInfo/2);
	m_vecBestPlayers[3]->DrawNet(window, cxInfo/2, cxInfo, cyInfo, cyInfo/2);
}
void CController::Render(sf::RenderWindow &window)
{
	if(CParams::bRender)
	{ 
	for (auto s : m_vecEnemies)
	{
		if (s->isAlive())
		{
			window.draw(*s);
			s->renderHUD(window);
		}
	}
	for (auto s : m_vecPlayers)
	{
		if (s->isAlive())
		{		
			window.draw(*s);
			s->renderHUD(window);
			if(show_sensors) RenderSensors(window);
			
		}
	}
	}
	window.draw(gen);
	
}
//------------------------------------Render()--------------------------------------
//
//----------------------------------------------------------------------------------
/*
void CController::Render(HDC &surface)
{
	//do not render if running at accelerated speed
	if (!m_bFastRender)
	{   
    string s = "Generation: " + itos(m_iGenerations);
	  TextOut(surface, 5, 0, s.c_str(), s.size());
    
    //select in the blue pen
    m_OldPen = (HPEN)SelectObject(surface, m_BluePen);
    
    if (m_bRenderBest)
    {  		
      //render the best sweepers memory cells
      m_vecBestPlayers[m_iViewThisSweeper]->Render(surface);

      //render the best sweepers from the last generation
      RenderSweepers(surface, m_vecBestSweepers);

      //render the best sweepers sensors
      RenderSensors(surface, m_vecBestSweepers);
    }
      
    else
    {
		  //render the sweepers
      RenderSweepers(surface, m_vecSweepers);
    }

    SelectObject(surface, m_OldPen);

    //render the objects
    for (int i=0; i<NumObjectVerts; i+=2)
    {
      MoveToEx(surface, m_ObjectsVB[i].x, m_ObjectsVB[i].y, NULL);

      LineTo(surface, m_ObjectsVB[i+1].x, m_ObjectsVB[i+1].y);
    }

	}//end if
 
  else
  {    
    PlotStats(surface);

    RECT sr;
    sr.top    = m_cyClient-50;
    sr.bottom = m_cyClient;
    sr.left   = 0;
    sr.right  = m_cxClient;

    //render the species chart
    m_pPop->RenderSpeciesInfo(surface, sr);

  }


//------------------------- RenderSweepers -------------------------------
//
//  given a vector of sweepers this function renders them to the screen
//------------------------------------------------------------------------
void CController::RenderSweepers(HDC &surface, vector<CMinesweeper> &sweepers)
{
  for (int i=0; i<sweepers.size(); ++i)
	{
    
    //if they have crashed into an obstacle draw
    if ( sweepers[i].Collided())
    {
      SelectObject(surface, m_RedPen);
    }

    else
    {
      SelectObject(surface, m_BluePen);
    }
      
    //grab the sweeper vertices
	  vector<SPoint> sweeperVB = m_SweeperVB;

	  //transform the vertex buffer
	  sweepers[i].WorldTransform(sweeperVB, sweepers[i].Scale());

	  //draw the sweeper left track
		MoveToEx(surface, (int)sweeperVB[0].x, (int)sweeperVB[0].y, NULL);

		for (int vert=1; vert<4; ++vert)
		{
		  LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
		}

    LineTo(surface, (int)sweeperVB[0].x, (int)sweeperVB[0].y);

    //draw the sweeper right track
	  MoveToEx(surface, (int)sweeperVB[4].x, (int)sweeperVB[4].y, NULL);

		for (int vert=5; vert<8; ++vert)
		{
	    LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
		}

    LineTo(surface, (int)sweeperVB[4].x, (int)sweeperVB[4].y);

    MoveToEx(surface, (int)sweeperVB[8].x, (int)sweeperVB[8].y, NULL);
    LineTo(surface, (int)sweeperVB[9].x, (int)sweeperVB[9].y);

    MoveToEx(surface, (int)sweeperVB[10].x, (int)sweeperVB[10].y, NULL);

    for (int vert=11; vert<16; ++vert)
    {
		   LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
		}
	}//next sweeper
}
*/
//----------------------------- RenderSensors ----------------------------
//
//  renders the sensors of a given vector of sweepers
//------------------------------------------------------------------------
void CController::RenderSensors(sf::RenderWindow &window)
{
   //render the sensors
	sf::Color col;
    for (int i=0; i<m_vecPlayers.size(); ++i)
    {
      //grab each sweepers sensor data
      vector<SPoint> tranSensors    = m_vecPlayers[i]->Sensors();
	  vector<SPoint> tranColSensors = m_vecPlayers[i]->ColSensors();
      vector<double> SensorReadings = m_vecPlayers[i]->SensorReadings();
	  vector<bool> ColSensorReadings = m_vecPlayers[i]->ColSensorReadings();
      vector<double> MemoryReadings = m_vecPlayers[i]->MemoryReadings();

	  
      for (int sr=0; sr<tranSensors.size(); ++sr)
      {
       if (SensorReadings[sr] > 0)
        {
			col = sf::Color(255.0f, 255.0f*SensorReadings[sr],0.0f,255.0f);
        }
	   else
	   {
		   col = sf::Color(255.0f, 255.0f, 0.0f, 255.0f);
	   }

       
        
        //make sure we clip the drawing of the sensors or we will get
        //unwanted artifacts appearing
		sf::Vertex line[2];
		sf::RectangleShape rect;
        if (!((fabs(m_vecPlayers[i]->getPosition().x - tranSensors[sr].x) >
              (CParams::dSensorRange+50))||
              (fabs(m_vecPlayers[i]->getPosition().y - tranSensors[sr].y) >
              (CParams::dSensorRange+50))))
        {
        
    
		  sf::Vertex line[] =
		  {
			  sf::Vertex(sf::Vector2f(m_vecPlayers[i]->getPosition().x, m_vecPlayers[i]->getPosition().y)),
			  sf::Vertex(sf::Vector2f((int)tranSensors[sr].x, (int)tranSensors[sr].y))
		  };

		  
          //render the cell sensors
		  
		  rect.setFillColor(col);
		  rect.setPosition(Vector2f(tranSensors[sr].x, tranSensors[sr].y));
		  rect.setSize(Vector2f(15.0f,15.0f));
		  window.draw(line, 2, sf::Lines);
		  window.draw(rect);
          if (MemoryReadings[sr] < 0)
          {

          //  FillRect(surface, &rect, m_BlueBrush);
          }

          else
          {
          //  FillRect(surface, &rect, m_RedBrush);
          }
          
        }
		
      }
	  for (int sr = 0; sr < tranColSensors.size(); ++sr)
	  {
		  if (ColSensorReadings[sr] == true)
		  {
			  col = sf::Color(0.0f, 255.0f, 0.0f, 255.0f);
		  }

		  else
		  {
			  col = sf::Color(0.0f, 127.5f, 127.5f,255.0f);
		  }

		  //make sure we clip the drawing of the sensors or we will get
		  //unwanted artifacts appearing
		  if (!((fabs(m_vecPlayers[i]->getPosition().x - tranColSensors[sr].x) >
			  (CParams::dSensorRange/3.0f + 1)) ||
			  (fabs(m_vecPlayers[i]->getPosition().y - tranColSensors[sr].y) >
			  (CParams::dSensorRange/3.0f + 1))))
		  {


			  sf::Vertex line[] =
			  {
				  sf::Vertex(sf::Vector2f(m_vecPlayers[i]->getPosition().x, m_vecPlayers[i]->getPosition().y)),
				  sf::Vertex(sf::Vector2f((int)tranColSensors[sr].x, (int)tranColSensors[sr].y))
			  };

			  window.draw(line, 2, sf::Lines);
			  //render the cell sensors
			  sf::RectangleShape rect;
			  rect.setFillColor(col);
			  rect.setPosition(Vector2f(tranColSensors[sr].x, tranColSensors[sr].y));
			  rect.setSize(Vector2f(10.0f, 10.0f));

			  window.draw(rect);
			  

		  }
	  }
    }   
}



//--------------------------PlotStats-------------------------------------
//
//  Given a surface to draw on this function displays some simple stats 
//------------------------------------------------------------------------
void CController::PlotStats(HDC surface)const
{
    string s = "Generation:              " + itos(m_iGenerations);
	  TextOut(surface, 5, 25, s.c_str(), s.size());
    
    s = "Num Species:          " + itos(m_pPop->NumSpecies());
	  TextOut(surface, 5, 45, s.c_str(), s.size());

    s = "Best Fitness so far: " + ftos(m_pPop->BestEverFitness());
    TextOut(surface, 5, 5, s.c_str(), s.size());
}


//------------------------------- GetFitnessScores -----------------------
//
//  returns a std::vector containing the genomes fitness scores
//------------------------------------------------------------------------
vector<double> CController::GetFitnessScores()const
{
  vector<double> scores;

  for (int i=0; i< m_vecPlayers.size(); ++i)
  {
     scores.push_back(m_vecPlayers[i]->Fitness());
  }
  return scores;
}