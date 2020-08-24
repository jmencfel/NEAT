#pragma once
#include "Player.h"

class HumanPlayer : public Player
{
public:
	HumanPlayer();
	bool isPlayer() override;
	void Update(const float &dt) override;
};