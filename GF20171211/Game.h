#pragma once

#include "SDL.h"
#include "SDL_TTF.h"

class GameStateMachine;

class Game {
public:
	~Game() {}

	bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void render();
	void update();
	bool running() { return m_bRunning; }
	void handleEvents();
	void clean();
	void quit() { m_bRunning = false; }

	static Game* Instance()
	{
		if (s_plnstance == 0) {
			s_plnstance = new Game();
			return s_plnstance;
		}
		return s_plnstance;
	}

	SDL_Renderer* getRenderer() const { return m_pRenderer; }
	GameStateMachine* getStateMachine() { return m_pGameStateMachine; }

	//// 초기화된 윈도우, 폰트, 컬러 값을 반환해주는 함수
	SDL_Window* getWindow() { return m_pWindow; }
	TTF_Font* getFont() { return font; }
	SDL_Color getColor() { return color; }

	//// 최고 기록과 현재 기록을 반환, 세팅해주는 함수
	int getBestrecord() { return best_Record; }
	int getCurrentrecord() { return current_Record; }
	void setBestrecord(int Record) { best_Record = Record; }
	void setCurrentrecord(int Record) { current_Record = Record; }

private:
	//// 최고 기록과 현재 기록을 기록해주는 변수
	int best_Record;
	int current_Record;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	TTF_Font* font;
	SDL_Color color;
	
	bool m_bRunning;

	Game() {}
	static Game* s_plnstance;

	GameStateMachine* m_pGameStateMachine;
};
typedef Game TheGame;