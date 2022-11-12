#pragma once

#include "SDLGameState.h"

class GameObject;

class GameOverState :public SDLGameState {
public:
	virtual bool onEnter();
	virtual bool onExit();
	virtual void render();

	virtual std::string getStateID()const { return s_gameOverID; }

	static GameOverState* Instance()
	{
		if (s_plnstance == 0)
			s_plnstance = new GameOverState();

		return s_plnstance;
	}

private:
	static void s_gameOverToMain();
	static void s_restartPlay();
	static const std::string s_gameOverID;
	static GameOverState* s_plnstance;

	GameOverState() {}
};
