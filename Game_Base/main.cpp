#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Invader.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "Game.h"
#include "levelsystem.h"
#include <windows.h>
#include <cstdio>
#include <math.h>

#include "utils.h"
#include "CController.h"
#include "CTimer.h"
#include "resource.h"
#include "CParams.h"
#include <omp.h>

using namespace sf;
using namespace std;

//The controller class for this simulation
CController*	g_pController = NULL;
bool lock_keys = false;
CParams   g_Params;
int iExperimentID = 0;
sf::View camera;
//global handle to the info window
HWND g_hwndInfo = NULL;

//global handle to the main window
HWND g_hwndMain = NULL;
float key_delay = 0.3f;
sf::Vector2f cam_pos = sf::Vector2f(640, 360);
Texture spritesheet, player_spritesheet;
Font font;
float idle_time = 0;
void Cleanup()
{
	if (g_pController)

		delete g_pController;
}

void Load()
{
	g_Params.Initialize();
	if(!spritesheet.loadFromFile("res/img/invaders_sheet.png"))
	{
		cerr << "Failed to load spritesheet!" << endl;
	}
	if (!player_spritesheet.loadFromFile("res/img/body.png"))
	{
		cerr << "Failed to load spritesheet!" << endl;
	}
	if (!font.loadFromFile("res/fonts/font.ttf")) {
		cout << "Cannot load font!" << endl;
	}
	switch (CParams::iMapID)
	{
	case 0:
		ls::loadLevelFile("res/levels/stripe.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 1:
		ls::loadLevelFile("res/levels/mirror_stripe.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 2:
		ls::loadLevelFile("res/levels/double_stripe.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 3:
		ls::loadLevelFile("res/levels/double_stripe2.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 4:
		ls::loadLevelFile("res/levels/double_stripe3.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 5:
		ls::loadLevelFile("res/levels/double_stripe4.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 6:
		ls::loadLevelFile("res/levels/double_stripe5.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 7:
		ls::loadLevelFile("res/levels/double_stripe7.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 10:
		ls::loadLevelFile("res/levels/new_stripe.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 11:
		ls::loadLevelFile("res/levels/new_double_stripe.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	case 12:
		ls::loadLevelFile("res/levels/new_cross.txt", 32.0f, ceil(CParams::iPopSize / 4), 1);
		break;
	default:
		break;
	}

	//setup the controller
	g_pController = new CController(1280, 720);
	
	
	
}

void Update(RenderWindow &window)
{
	/*if (g_pController->isExperimentComplete())
	{
		g_pController->~CController();
		if (iExperimentID == 0)
		{
			g_Params.Initialize2();
			iExperimentID++;
		}
		else if (iExperimentID == 1)
		{
			g_Params.Initialize3();
			iExperimentID++;
		}
		else if (iExperimentID == 2)
		{
			g_Params.Initialize4();
			iExperimentID++;
		}
		else if (iExperimentID == 3)
		{
			window.close();
		}
		g_pController= new CController(1280, 720);
		
	}*/
	static Clock clock;
	float dt = clock.restart().asSeconds();
	idle_time += dt;
	key_delay -= dt;
	 dt *= CParams::fSpeedUp;
	Event event;	
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
			return;
		}
	}
	//g_pController->Update();
	
	if (Keyboard::isKeyPressed(Keyboard::F10))
	{
		window.close();
		return;
	}
	
	camera.setCenter(cam_pos);
	window.setView(camera);
	g_pController->Update(dt);


	for (int i=0; i<m_vecEnemies.size(); i++)
	{
		m_vecEnemies[i]->Update(dt);
	}
	if (key_delay <= 0 && Keyboard::isKeyPressed(Keyboard::Multiply))
	{
		lock_keys = (lock_keys) ? false : true;
		key_delay = 0.5f;
	}
	if(!lock_keys)
	{ 
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		cam_pos.x -= 500.0f*dt;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		cam_pos.x += 500.0f*dt;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		cam_pos.y -= 500.0f*dt;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		cam_pos.y += 500.0f*dt;
	}
	if (key_delay <=0 && Keyboard::isKeyPressed(Keyboard::R))
	{
		CParams::bRender = (CParams::bRender) ? false : true;
		key_delay = 0.5f;
	}
	}
	

}

void Render(RenderWindow &window)
{
	if (CParams::bRender)
	{
		ls::Render(window);
	}
		g_pController->Render(window);
	
//	std::cout << "Enemies: " << m_vecEnemies.size() << "    NEATs: " << m_vecPlayers.size() << endl;
	
	
}
void RenderPhenotypes(RenderWindow &window)
{
	
	g_pController->RenderNetworks(window);

	//	std::cout << "Enemies: " << m_vecEnemies.size() << "    NEATs: " << m_vecPlayers.size() << endl;


}
int main()
{

	FreeConsole();
	camera.setCenter(cam_pos); //in constructor
	camera.setSize(gameWidth, gameHeight); //in constructor
	RenderWindow window(VideoMode(gameWidth, gameHeight), "NEAT");
	RenderWindow phenotypes(VideoMode(gameWidth, gameHeight), "Best phenotypes", sf::Style::Titlebar);
	window.setView(camera);
	Load();
	while (window.isOpen())
	{
		window.clear();
		phenotypes.clear();
		Update(window);
		RenderPhenotypes(phenotypes);
		Render(window);
		window.display();
		phenotypes.display();
	}
	return 0;
}