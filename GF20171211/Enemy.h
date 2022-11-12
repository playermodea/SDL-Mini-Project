#pragma once

#include "SDLGameObject.h"

class Enemy : public SDLGameObject {
public:
	Enemy(const LoaderParams* pParams);
	virtual void update();
	virtual void onBody();
	virtual void onLeg();
	virtual void isAttack();
	
	void isDie();
	void handleInput();
};