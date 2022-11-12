#pragma once

#include "SDLGameState.h"

class GameObject;

class PauseState :public SDLGameState {
public:
	virtual bool onEnter();
	virtual bool onExit();
	virtual std::string getStateID() const { return s_pauseID; }

	static PauseState* Instance()
	{
		if (s_plnstance == 0)
			s_plnstance = new PauseState();

		return s_plnstance;
	}

private:
	static void s_pauseToMain();
	static void s_resumePlay();
	static const std::string s_pauseID;
	static PauseState* s_plnstance;

	PauseState() {};
};