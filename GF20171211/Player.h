#pragma once

#include "SDLGameObject.h"

class Player :public SDLGameObject {
public:
	Player(const LoaderParams* pParams);
	virtual void update();
	virtual void onBody();
	virtual void onLeg();
	virtual void isAttack();

	int getBatCount() { return bat_Count; }
	void setMode() { bat_Mode = true; bat_Count = 10; }
	void handleInput();

private:
	int window_w, window_h;
	int stamina;
	int animation_Speed;
	int bat_Count;
	int hit_pivotX, hit_pivotY;
	
	bool running;
	bool bat_Mode;
};