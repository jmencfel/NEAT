#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "NEATPlayer.h"


constexpr uint16_t gameWidth = 1280;
constexpr uint16_t gameHeight = 720;
constexpr uint16_t invaders_rows = 1;
constexpr uint16_t invaders_columns = 1;

extern float idle_time;
extern bool lock_keys;
extern sf::Texture spritesheet, player_spritesheet;
extern std::vector<Player*> m_vecEnemies;
extern std::vector<NEATPlayer*> m_vecPlayers;
extern sf::Font font;
extern sf::Vector2f cam_pos;

