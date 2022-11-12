#pragma once

#include "SDLGameState.h"

class MenuState : public SDLGameState {
public:
	virtual bool onEnter();
	virtual bool onExit();
	virtual std::string getStateID() const { return s_menuID; }

	static MenuState* Instance()
	{
		if (s_plnstance == 0)
			s_plnstance = new MenuState();

		return s_plnstance;
	}

private:
	static const std::string s_menuID;
	static void s_menuToPlay();
	static void s_exitFromMenu();
	static MenuState* s_plnstance;

	MenuState() {}
};