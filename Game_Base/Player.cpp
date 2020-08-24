#include "Player.h"
#include "Game.h"
using namespace sf;
using namespace std;


Player::Player() 
{
alive = true;

defending = false;
setTexture(player_spritesheet);
setTextureRect(_sprite);
frame = 0;
setOrigin({ 32.0f, 32.0f });
text_time_alive.setFont(font);
text_time_alive.setColor(sf::Color::Red);
text_time_alive.setCharacterSize(15.0f);
text_kills.setFont(font);
text_kills.setCharacterSize(15.0f);
text_kills.setColor(sf::Color::Red);
text_last_fitness.setFont(font);
text_last_fitness.setCharacterSize(15.0f);
text_last_fitness.setColor(sf::Color::Red);

walkingAnimationDown.push_back(sf::IntRect(0, 128, 64, 64));
walkingAnimationDown.push_back(sf::IntRect(64, 128, 64, 64));
walkingAnimationDown.push_back(sf::IntRect(128, 128, 64, 64));
walkingAnimationDown.push_back(sf::IntRect(192, 128, 64, 64));
walkingAnimationDown.push_back(sf::IntRect(256, 128, 64, 64));
walkingAnimationDown.push_back(sf::IntRect(320, 128, 64, 64));
walkingAnimationDown.push_back(sf::IntRect(384, 128, 64, 64));
walkingAnimationDown.push_back(sf::IntRect(448, 128, 64, 64));
walkingAnimationDown.push_back(sf::IntRect(512, 128, 64, 64));


walkingAnimationUp.push_back(sf::IntRect(0, 0, 64, 64));
walkingAnimationUp.push_back(sf::IntRect(64, 0, 64, 64));
walkingAnimationUp.push_back(sf::IntRect(128, 0, 64, 64));
walkingAnimationUp.push_back(sf::IntRect(192, 0, 64, 64));
walkingAnimationUp.push_back(sf::IntRect(256, 0, 64, 64));
walkingAnimationUp.push_back(sf::IntRect(320, 0, 64, 64));
walkingAnimationUp.push_back(sf::IntRect(384, 0, 64, 64));
walkingAnimationUp.push_back(sf::IntRect(448, 0, 64, 64));
walkingAnimationUp.push_back(sf::IntRect(512, 0, 64, 64));



walkingAnimationRight.push_back(sf::IntRect(0, 192, 64, 64));
walkingAnimationRight.push_back(sf::IntRect(64, 192, 64, 64));
walkingAnimationRight.push_back(sf::IntRect(128, 192, 64, 64));
walkingAnimationRight.push_back(sf::IntRect(192, 192, 64, 64));
walkingAnimationRight.push_back(sf::IntRect(256, 192, 64, 64));
walkingAnimationRight.push_back(sf::IntRect(320, 192, 64, 64));
walkingAnimationRight.push_back(sf::IntRect(384, 192, 64, 64));
walkingAnimationRight.push_back(sf::IntRect(448, 192, 64, 64));
walkingAnimationRight.push_back(sf::IntRect(512, 192, 64, 64));

walkingAnimationLeft.push_back(sf::IntRect(0, 64, 64, 64));
walkingAnimationLeft.push_back(sf::IntRect(64, 64, 64, 64));
walkingAnimationLeft.push_back(sf::IntRect(128, 64, 64, 64));
walkingAnimationLeft.push_back(sf::IntRect(192, 64, 64, 64));
walkingAnimationLeft.push_back(sf::IntRect(256, 64, 64, 64));
walkingAnimationLeft.push_back(sf::IntRect(320, 64, 64, 64));
walkingAnimationLeft.push_back(sf::IntRect(384, 64, 64, 64));
walkingAnimationLeft.push_back(sf::IntRect(448, 64, 64, 64));
walkingAnimationLeft.push_back(sf::IntRect(512, 64, 64, 64));



swordAnimationUp.push_back(sf::IntRect(0, 256, 64, 64));
swordAnimationUp.push_back(sf::IntRect(64, 256, 64, 64));
swordAnimationUp.push_back(sf::IntRect(128, 256, 64, 64));
swordAnimationUp.push_back(sf::IntRect(192, 256, 64, 64));
swordAnimationUp.push_back(sf::IntRect(256, 256, 64, 64));
swordAnimationUp.push_back(sf::IntRect(320, 256, 64, 64));

swordAnimationLeft.push_back(sf::IntRect(0, 320, 64, 64));
swordAnimationLeft.push_back(sf::IntRect(64, 320, 64, 64));
swordAnimationLeft.push_back(sf::IntRect(128, 320, 64, 64));
swordAnimationLeft.push_back(sf::IntRect(192, 320, 64, 64));
swordAnimationLeft.push_back(sf::IntRect(256, 320, 64, 64));
swordAnimationLeft.push_back(sf::IntRect(320, 320, 64, 64));

swordAnimationDown.push_back(sf::IntRect(0, 384, 64, 64));
swordAnimationDown.push_back(sf::IntRect(64, 384, 64, 64));
swordAnimationDown.push_back(sf::IntRect(128, 384, 64, 64));
swordAnimationDown.push_back(sf::IntRect(192, 384, 64, 64));
swordAnimationDown.push_back(sf::IntRect(256, 384, 64, 64));
swordAnimationDown.push_back(sf::IntRect(320, 384, 64, 64));

swordAnimationRight.push_back(sf::IntRect(0, 448, 64, 64));
swordAnimationRight.push_back(sf::IntRect(64, 448, 64, 64));
swordAnimationRight.push_back(sf::IntRect(128, 448, 64, 64));
swordAnimationRight.push_back(sf::IntRect(192, 448, 64, 64));
swordAnimationRight.push_back(sf::IntRect(256, 448, 64, 64));
swordAnimationRight.push_back(sf::IntRect(320, 448, 64, 64));

defendingAnimationUp.push_back(sf::IntRect(0, 512, 64, 64));
defendingAnimationUp.push_back(sf::IntRect(64, 512, 64, 64));
defendingAnimationUp.push_back(sf::IntRect(128, 512, 64, 64));
defendingAnimationUp.push_back(sf::IntRect(192, 512, 64, 64));
defendingAnimationUp.push_back(sf::IntRect(256, 512, 64, 64));
defendingAnimationUp.push_back(sf::IntRect(320, 512, 64, 64));
defendingAnimationUp.push_back(sf::IntRect(384, 512, 64, 64));
defendingAnimationUp.push_back(sf::IntRect(448, 512, 64, 64));
defendingAnimationUp.push_back(sf::IntRect(512, 512, 64, 64));

defendingAnimationLeft.push_back(sf::IntRect(0, 576, 64, 64));
defendingAnimationLeft.push_back(sf::IntRect(64, 576, 64, 64));
defendingAnimationLeft.push_back(sf::IntRect(128, 576, 64, 64));
defendingAnimationLeft.push_back(sf::IntRect(192, 576, 64, 64));
defendingAnimationLeft.push_back(sf::IntRect(256, 576, 64, 64));
defendingAnimationLeft.push_back(sf::IntRect(320, 576, 64, 64));
defendingAnimationLeft.push_back(sf::IntRect(384, 576, 64, 64));
defendingAnimationLeft.push_back(sf::IntRect(448, 576, 64, 64));
defendingAnimationLeft.push_back(sf::IntRect(512, 576, 64, 64));

defendingAnimationDown.push_back(sf::IntRect(0, 640, 64, 64));
defendingAnimationDown.push_back(sf::IntRect(64, 640, 64, 64));
defendingAnimationDown.push_back(sf::IntRect(128, 640, 64, 64));
defendingAnimationDown.push_back(sf::IntRect(192, 640, 64, 64));
defendingAnimationDown.push_back(sf::IntRect(256, 640, 64, 64));
defendingAnimationDown.push_back(sf::IntRect(320, 640, 64, 64));
defendingAnimationDown.push_back(sf::IntRect(384, 640, 64, 64));
defendingAnimationDown.push_back(sf::IntRect(448, 640, 64, 64));
defendingAnimationDown.push_back(sf::IntRect(512, 640, 64, 64));

defendingAnimationRight.push_back(sf::IntRect(0, 704, 64, 64));
defendingAnimationRight.push_back(sf::IntRect(64, 704, 64, 64));
defendingAnimationRight.push_back(sf::IntRect(128, 704, 64, 64));
defendingAnimationRight.push_back(sf::IntRect(192, 704, 64, 64));
defendingAnimationRight.push_back(sf::IntRect(256, 704, 64, 64));
defendingAnimationRight.push_back(sf::IntRect(320, 704, 64, 64));
defendingAnimationRight.push_back(sf::IntRect(384, 704, 64, 64));
defendingAnimationRight.push_back(sf::IntRect(448, 704, 64, 64));
defendingAnimationRight.push_back(sf::IntRect(512, 704, 64, 64));




};

Player::Player(IntRect ir) : Sprite()
{
	alive = true;
	defending = false;
	_sprite = ir;
	setTexture(player_spritesheet);
	setTextureRect(_sprite);
	frame = 0;
	text_time_alive.setFont(font);
	text_time_alive.setCharacterSize(15.0f);
	text_kills.setFont(font);
	text_kills.setCharacterSize(15.0f);
	text_last_fitness.setFont(font);
	text_last_fitness.setCharacterSize(15.0f);
	setOrigin({ 32.0f, 32.0f });
	
	walkingAnimationDown.push_back(sf::IntRect(0, 128, 64, 64));
	walkingAnimationDown.push_back(sf::IntRect(64, 128, 64, 64));
	walkingAnimationDown.push_back(sf::IntRect(128, 128, 64, 64));
	walkingAnimationDown.push_back(sf::IntRect(192, 128, 64, 64));
	walkingAnimationDown.push_back(sf::IntRect(256, 128, 64, 64));
	walkingAnimationDown.push_back(sf::IntRect(320, 128, 64, 64));
	walkingAnimationDown.push_back(sf::IntRect(384, 128, 64, 64));
	walkingAnimationDown.push_back(sf::IntRect(448, 128, 64, 64));
	walkingAnimationDown.push_back(sf::IntRect(512, 128, 64, 64));


	walkingAnimationUp.push_back(sf::IntRect(0, 0, 64, 64));
	walkingAnimationUp.push_back(sf::IntRect(64, 0, 64, 64));
	walkingAnimationUp.push_back(sf::IntRect(128, 0, 64, 64));
	walkingAnimationUp.push_back(sf::IntRect(192, 0, 64, 64));
	walkingAnimationUp.push_back(sf::IntRect(256, 0, 64, 64));
	walkingAnimationUp.push_back(sf::IntRect(320, 0, 64, 64));
	walkingAnimationUp.push_back(sf::IntRect(384, 0, 64, 64));
	walkingAnimationUp.push_back(sf::IntRect(448, 0, 64, 64));
	walkingAnimationUp.push_back(sf::IntRect(512, 0, 64, 64));



	walkingAnimationRight.push_back(sf::IntRect(0, 192, 64, 64));
	walkingAnimationRight.push_back(sf::IntRect(64, 192, 64, 64));
	walkingAnimationRight.push_back(sf::IntRect(128, 192, 64, 64));
	walkingAnimationRight.push_back(sf::IntRect(192, 192, 64, 64));
	walkingAnimationRight.push_back(sf::IntRect(256, 192, 64, 64));
	walkingAnimationRight.push_back(sf::IntRect(320, 192, 64, 64));
	walkingAnimationRight.push_back(sf::IntRect(384, 192, 64, 64));
	walkingAnimationRight.push_back(sf::IntRect(448, 192, 64, 64));
	walkingAnimationRight.push_back(sf::IntRect(512, 192, 64, 64));

	walkingAnimationLeft.push_back(sf::IntRect(0, 64, 64, 64));
	walkingAnimationLeft.push_back(sf::IntRect(64, 64, 64, 64));
	walkingAnimationLeft.push_back(sf::IntRect(128, 64, 64, 64));
	walkingAnimationLeft.push_back(sf::IntRect(192, 64, 64, 64));
	walkingAnimationLeft.push_back(sf::IntRect(256, 64, 64, 64));
	walkingAnimationLeft.push_back(sf::IntRect(320, 64, 64, 64));
	walkingAnimationLeft.push_back(sf::IntRect(384, 64, 64, 64));
	walkingAnimationLeft.push_back(sf::IntRect(448, 64, 64, 64));
	walkingAnimationLeft.push_back(sf::IntRect(512, 64, 64, 64));



	swordAnimationUp.push_back(sf::IntRect(0, 256, 64, 64));
	swordAnimationUp.push_back(sf::IntRect(64, 256, 64, 64));
	swordAnimationUp.push_back(sf::IntRect(128, 256, 64, 64));
	swordAnimationUp.push_back(sf::IntRect(192, 256, 64, 64));
	swordAnimationUp.push_back(sf::IntRect(256, 256, 64, 64));
	swordAnimationUp.push_back(sf::IntRect(320, 256, 64, 64));

	swordAnimationLeft.push_back(sf::IntRect(0, 320, 64, 64));
	swordAnimationLeft.push_back(sf::IntRect(64, 320, 64, 64));
	swordAnimationLeft.push_back(sf::IntRect(128, 320, 64, 64));
	swordAnimationLeft.push_back(sf::IntRect(192, 320, 64, 64));
	swordAnimationLeft.push_back(sf::IntRect(256, 320, 64, 64));
	swordAnimationLeft.push_back(sf::IntRect(320, 320, 64, 64));

	swordAnimationDown.push_back(sf::IntRect(0, 384, 64, 64));
	swordAnimationDown.push_back(sf::IntRect(64, 384, 64, 64));
	swordAnimationDown.push_back(sf::IntRect(128, 384, 64, 64));
	swordAnimationDown.push_back(sf::IntRect(192, 384, 64, 64));
	swordAnimationDown.push_back(sf::IntRect(256, 384, 64, 64));
	swordAnimationDown.push_back(sf::IntRect(320, 384, 64, 64));

	swordAnimationRight.push_back(sf::IntRect(0, 448, 64, 64));
	swordAnimationRight.push_back(sf::IntRect(64, 448, 64, 64));
	swordAnimationRight.push_back(sf::IntRect(128, 448, 64, 64));
	swordAnimationRight.push_back(sf::IntRect(192, 448, 64, 64));
	swordAnimationRight.push_back(sf::IntRect(256, 448, 64, 64));
	swordAnimationRight.push_back(sf::IntRect(320, 448, 64, 64));
	defendingAnimationUp.push_back(sf::IntRect(0, 512, 64, 64));
	defendingAnimationUp.push_back(sf::IntRect(64, 512, 64, 64));
	defendingAnimationUp.push_back(sf::IntRect(128, 512, 64, 64));
	defendingAnimationUp.push_back(sf::IntRect(192, 512, 64, 64));
	defendingAnimationUp.push_back(sf::IntRect(256, 512, 64, 64));
	defendingAnimationUp.push_back(sf::IntRect(320, 512, 64, 64));
	defendingAnimationUp.push_back(sf::IntRect(384, 512, 64, 64));
	defendingAnimationUp.push_back(sf::IntRect(448, 512, 64, 64));
	defendingAnimationUp.push_back(sf::IntRect(512, 512, 64, 64));

	defendingAnimationLeft.push_back(sf::IntRect(0, 576, 64, 64));
	defendingAnimationLeft.push_back(sf::IntRect(64, 576, 64, 64));
	defendingAnimationLeft.push_back(sf::IntRect(128, 576, 64, 64));
	defendingAnimationLeft.push_back(sf::IntRect(192, 576, 64, 64));
	defendingAnimationLeft.push_back(sf::IntRect(256, 576, 64, 64));
	defendingAnimationLeft.push_back(sf::IntRect(320, 576, 64, 64));
	defendingAnimationLeft.push_back(sf::IntRect(384, 576, 64, 64));
	defendingAnimationLeft.push_back(sf::IntRect(448, 576, 64, 64));
	defendingAnimationLeft.push_back(sf::IntRect(512, 576, 64, 64));

	defendingAnimationDown.push_back(sf::IntRect(0, 640, 64, 64));
	defendingAnimationDown.push_back(sf::IntRect(64, 640, 64, 64));
	defendingAnimationDown.push_back(sf::IntRect(128, 640, 64, 64));
	defendingAnimationDown.push_back(sf::IntRect(192, 640, 64, 64));
	defendingAnimationDown.push_back(sf::IntRect(256, 640, 64, 64));
	defendingAnimationDown.push_back(sf::IntRect(320, 640, 64, 64));
	defendingAnimationDown.push_back(sf::IntRect(384, 640, 64, 64));
	defendingAnimationDown.push_back(sf::IntRect(448, 640, 64, 64));
	defendingAnimationDown.push_back(sf::IntRect(512, 640, 64, 64));

	defendingAnimationRight.push_back(sf::IntRect(0, 704, 64, 64));
	defendingAnimationRight.push_back(sf::IntRect(64, 704, 64, 64));
	defendingAnimationRight.push_back(sf::IntRect(128, 704, 64, 64));
	defendingAnimationRight.push_back(sf::IntRect(192, 704, 64, 64));
	defendingAnimationRight.push_back(sf::IntRect(256, 704, 64, 64));
	defendingAnimationRight.push_back(sf::IntRect(320, 704, 64, 64));
	defendingAnimationRight.push_back(sf::IntRect(384, 704, 64, 64));
	defendingAnimationRight.push_back(sf::IntRect(448, 704, 64, 64));
	defendingAnimationRight.push_back(sf::IntRect(512, 704, 64, 64));
	
};
bool Player::isAlive() const
{
	return alive;
}
bool Player::isHUD() const
{
	return show_hud;
}
bool Player::isDefending() const
{
	return defending;
}
void Player::Die()
{
	alive = false;
}
void Player::getHit(float x)
{
	health -= x;
	damage_taken += x;
	idle_time = 0.0f;
}
void Player::Push(float x, float y)
{
	Push(x, y, true);
}
void Player::parry()
{
	defendDelay = 2.0f;
}
void Player::Push(float x, float y, bool f)
{
	pushed = true;
	push_x = 10.0f*x*CParams::fSpeedUp;
	push_y = 10.0f*y*CParams::fSpeedUp;
	pushTimer = 0.1f;
	if(f)
		setColor(sf::Color::Red);
	else
	{
		
		setColor(sf::Color::Yellow);
	}
}
void Player::renderHUD(sf::RenderWindow &window)
{
	if (show_hud)
	{
		window.draw(text_time_alive);
		window.draw(text_kills);
		if(is_best)
		window.draw(text_last_fitness);
	}
}
void Player::Reset()
{
	alive = true;
	pushed = false;
	push_x = 0;
	push_y = 0;
	health = 100;
	t_idle = 0.0f;
	int kills = 0;
	isMoving = false;
	setColor(default_color);
	//and the fitness
	setPosition(start_positon);
	defending = false;
	isAttacking = false;
	animation_incomplete = false;
	time_alive = 0.0f;
	
}
void Player::Reset(Vector2f pos)
{
	alive = true;
	pushed = false;
	push_x = 0;
	push_y = 0;
	health = 100;
	t_idle = 0.0f;
	int kills = 0;
	isMoving = false;
	setColor(default_color);
	//and the fitness
	setPosition(pos);
	defending = false;
	isAttacking = false;
	animation_incomplete = false;
	time_alive = 0.0f;

}
bool Player::validMove(const sf::Vector2f &pos)
{
	if (isNEAT)
	{
		return (LevelSystem::getTileAt(pos) != ls::WALL);
	}
	else
	{
		return (LevelSystem::getTileAt(pos) != ls::WALL && LevelSystem::getTileAt(pos) != ls::WALL2);
	}
}
void Player::Update(const float &dt)
{
	if (show_hud)
	{
		text_time_alive.setPosition(getPosition() - getOrigin() + Vector2f(0, -40.0f));
		text_kills.setPosition(getPosition() - getOrigin() + Vector2f(0, -20.0f));
		text_time_alive.setString("Time: " + std::to_string(time_alive));
		text_kills.setString("Kills:" + std::to_string(kills));

	}
	if (alive)
	{
		time_alive += dt/CParams::fSpeedUp;
		if (health <= 0)
		{
			Die();
		}
		if (pushed)
		{
			pushTimer -= 1.0f*CParams::fSpeedUp / 90.0f;
			if (validMove(getPosition() + Vector2f(push_x, push_y)))
			{
				move(push_x, push_y);
			}

		}
		if (pushed && pushTimer <= 0)
		{
			pushTimer = 0.1f;
			pushed = false;
			setColor(default_color);
		}
		AnimationCounter -= dt;
		if (frame >= walkingAnimationUp.size()) frame = 0;

		if (CParams::bRender)
		{
			if (!isAttacking)
			{
				if (!defending)
				{
					if (!isMoving)
						frame = 0;
					if (facing == 1 && AnimationCounter <= 0.0f)
					{

						setTextureRect(walkingAnimationUp[frame]);

						frame++;
						AnimationCounter = AnimationDelay;
					}
					else if (facing == 2 && AnimationCounter <= 0.0f)
					{

						setTextureRect(walkingAnimationRight[frame]);

						frame++;
						AnimationCounter = AnimationDelay;
					}
					else if (facing == 3 && AnimationCounter <= 0.0f)
					{

						setTextureRect(walkingAnimationDown[frame]);

						frame++;
						AnimationCounter = AnimationDelay;
					}
					else if (facing == 4 && AnimationCounter <= 0.0f)
					{

						setTextureRect(walkingAnimationLeft[frame]);

						frame++;
						AnimationCounter = AnimationDelay;
					}
				}
				else
				{
					if (!isMoving)
						frame = 0;
					if (facing == 1 && AnimationCounter <= 0.0f)
					{

						setTextureRect(defendingAnimationUp[frame]);

						frame++;
						AnimationCounter = AnimationDelay;
					}
					else if (facing == 2 && AnimationCounter <= 0.0f)
					{

						setTextureRect(defendingAnimationRight[frame]);

						frame++;
						AnimationCounter = AnimationDelay;
					}
					else if (facing == 3 && AnimationCounter <= 0.0f)
					{

						setTextureRect(defendingAnimationDown[frame]);

						frame++;
						AnimationCounter = AnimationDelay;
					}
					else if (facing == 4 && AnimationCounter <= 0.0f)
					{

						setTextureRect(defendingAnimationLeft[frame]);

						frame++;
						AnimationCounter = AnimationDelay;
					}
				}
			}
			if (isAttacking && animation_incomplete == false)
			{
				animation_incomplete = true;
			}
			if (animation_incomplete)
			{
				if (frame > 5)
				{
					animation_incomplete = false;
					isAttacking = false;
					frame = 0;
				}
				if (facing == 3 && AnimationCounter <= 0.0f)
				{

					setTextureRect(swordAnimationDown[frame]);
					frame++;
					AnimationCounter = 0.04f;
				}
				else if (facing == 1 && AnimationCounter <= 0.0f)
				{

					setTextureRect(swordAnimationUp[frame]);
					frame++;
					AnimationCounter = 0.04f;
				}
				else if (facing == 2 && AnimationCounter <= 0.0f)
				{

					setTextureRect(swordAnimationRight[frame]);
					frame++;
					AnimationCounter = 0.04f;
				}
				else if (facing == 4 && AnimationCounter <= 0.0f)
				{

					setTextureRect(swordAnimationLeft[frame]);
					frame++;
					AnimationCounter = 0.04f;
				}
			}
		}
	}

}
bool Player::isPlayer()
{
	return false;
}
Player::~Player() = default;
