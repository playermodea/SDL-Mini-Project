#pragma once

#include "SDLGameState.h"
#include "SDLGameObject.h"

class PlayState : public SDLGameState {
public:
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual bool onExit();
	virtual std::string getStateID() const { return s_playID; }

	bool checkCollision(SDLGameObject* p1, SDLGameObject* p2);

	static PlayState* Instance()
	{
		if (s_plnstance == 0)
			s_plnstance = new PlayState();

		return s_plnstance;
	}

private:
	static const std::string s_playID;
	static PlayState* s_plnstance;

	int kill_score;
	int start_Delay;
	int end_Delay;
	int clean_Time;

	PlayState() {}
};