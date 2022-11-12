#include "Game.h"
#include "InputHandler.h"
#include "GameStateMachine.h"
#include "MenuState.h"
#include "SDL_Mixer.h"
#include "SDL_TTF.h"
#include <time.h>

Game* Game::s_plnstance = 0;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (m_pWindow != 0)
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != 0)
				SDL_SetRenderDrawColor(m_pRenderer, 0, 153, 0, 255);
			else
				return false;
		}
		else
			return false;
	}
	/////////// 기록 관련 변수들 초기화
	best_Record = 0;
	current_Record = 0;

	////////// 폰트, 음악, 난수 생성 초기화
	TTF_Init();
	font = TTF_OpenFont("assets/font/malgun.ttf", 100);
	color = { 0,0,0 };

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_VolumeMusic(30);

	srand((unsigned int)time(NULL));
	//////////

	m_pGameStateMachine = new GameStateMachine();
	m_pGameStateMachine->changeState(MenuState::Instance());

	m_bRunning = true;
	return true;
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);
	m_pGameStateMachine->render();
	SDL_RenderPresent(m_pRenderer);
}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();
}

void Game::clean()
{
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

void Game::update()
{
	m_pGameStateMachine->update();
}