#pragma once
#include <SFML/Graphics.hpp>
#include "levelsystem.h"
#include "CParams.h"

class Player : public sf::Sprite
{
protected:
	bool isNEAT = false;
	sf::Text text_time_alive;
	sf::Text text_kills;
	sf::Text text_last_fitness;
	sf::IntRect _sprite;
	bool alive = true;
	bool show_hud = false;
	Player();
	int kills = 0;
	sf::Color default_color = sf::Color::White;
	float time_alive = 0.0f;
	float t_idle = 0.0f;
	/////////////////////////////ANIMATION AND SPRITES
	bool isMoving = false;
	bool pushed = false;
	bool defending = false;
	float pushTimer = 0.1f;
	sf::Vector2f start_positon;
	bool isAttacking = false;
	bool animation_incomplete = false;
//	std::shared_ptr<sf::Sprite> _sprite;
	int facing=3; //direction player/npc is facing 1 - north, then clockwise (1-4)
	float face_x = 0;
	float push_x = 0;
	bool is_best = false;
	float push_y = 0;
	float face_y = 0;
	float distance_walked = 0.0f;
	float fireTime = 0.0f;
	float defendDelay = 0.0f;
	float damage_taken = 0;
	float damage_done = 0;
	float health = 100;
	float max_health = 100;
	int frame = 0; //Current frame of player/npc animation (goes from 0 to 3)
	const float AnimationDelay = 0.05f ; //delay between frames of animation
	float AnimationCounter = 0.15f; //used as a count-down between frames, then set to AnimationDelay when frame changes
	std::vector<sf::IntRect> walkingAnimationDown;
	std::vector<sf::IntRect> walkingAnimationUp;
	std::vector<sf::IntRect> walkingAnimationRight;
	std::vector<sf::IntRect> walkingAnimationLeft;
	std::vector<sf::IntRect> swordAnimationDown;
	std::vector<sf::IntRect> swordAnimationUp;
	std::vector<sf::IntRect> swordAnimationRight;
	std::vector<sf::IntRect> swordAnimationLeft;
	std::vector<sf::IntRect> defendingAnimationUp;
	std::vector<sf::IntRect> defendingAnimationDown;
	std::vector<sf::IntRect> defendingAnimationRight;
	std::vector<sf::IntRect> defendingAnimationLeft;
public:
	bool isPushed() { return pushed; }
	bool isBest() { return is_best; }
	bool validMove(const sf::Vector2f &pos);
	int getTime() { return time_alive; }
	int getHealth() { return health; }
	int getKills() {return kills;}
	void addKill() {kills++;}
	void Reset();
	void Reset(sf::Vector2f pos);
	float destructionDelay = 0.5f;
	bool isAlive() const;
	bool isHUD() const;
	virtual void Die();
	virtual bool isPlayer();
	void getHit(float x);
	void Push(float x, float y);
	void Push(float x, float y, bool f);
	void parry();
	bool isDefending() const;
	void renderHUD(sf::RenderWindow &window);
	explicit Player(sf::IntRect ir);

	virtual ~Player() = 0;

	virtual void Update(const float &dt);
};
