#pragma once

#include "GameState.h"
#include "vector"

class GameStateMachine {
public:
	void changeState(GameState* pState);
	void pauseState(GameState* pState);
	void popState();
	void update();
	void render();

private:
	GameState* m_currentState;
	GameState* m_prevState;
};