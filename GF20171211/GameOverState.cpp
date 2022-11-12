#include "GameOverState.h"
#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"
#include "GameStateMachine.h"
#include "TextureManager.h"
#include "AnimatedGraphic.h"
#include "MenuButton.h"
#include "Background.h"
#include "SoundManager.h"
#include <iostream>

GameOverState* GameOverState::s_plnstance = NULL;
const std::string GameOverState::s_gameOverID = "GAMEOVER";

void GameOverState::s_gameOverToMain()
{
	TheGame::Instance()->getStateMachine()->changeState(MenuState::Instance());
}

void GameOverState::s_restartPlay()
{
	TheGame::Instance()->getStateMachine()->changeState(PlayState::Instance());
}

void GameOverState::render()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->draw();

	//// 최고 기록과 현재 기록 텍스트 출력
	std::string str_1 = std::string("Best Record: ") + std::to_string(TheGame::Instance()->getBestrecord());
	std::string str_2 = std::string("Current Record: ") + std::to_string(TheGame::Instance()->getCurrentrecord());
	TheTextureManager::Instance()->drawText(str_1, 560, 470, 250, 100, TheGame::Instance()->getFont(), TheGame::Instance()->getColor(), TheGame::Instance()->getRenderer());
	TheTextureManager::Instance()->drawText(str_2, 560, 620, 250, 100, TheGame::Instance()->getFont(), TheGame::Instance()->getColor(), TheGame::Instance()->getRenderer());
}

bool GameOverState::onEnter()
{
	TheSoundManger::Instance()->playGameover();		//// 배경 음악 재생

	/////// 텍스쳐 추가
	if (!TheTextureManager::Instance()->load("assets/background/gameover_background.png", "gameover_background", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/background/gameover_player.png", "player_background", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/gameover.png", "gameovertext", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/main.png", "mainbutton", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/restart.png", "restartbutton", TheGame::Instance()->getRenderer()))
		return false;

	GameObject* background = new Background(new LoaderParams(0, 0, 1920, 1080, "gameover_background"));
	GameObject* background_2 = new Background(new LoaderParams(860, 320, 200, 94, "player_background"));
	GameObject* gameOverText = new AnimatedGraphic(new LoaderParams(865, 420, 190, 30, "gameovertext"), 2);
	GameObject* button1 = new MenuButton(new LoaderParams(860, 520, 200, 80, "mainbutton"), s_gameOverToMain);
	GameObject* button2 = new MenuButton(new LoaderParams(860, 620, 200, 80, "restartbutton"), s_restartPlay);

	m_gameObjects.push_back(background);
	m_gameObjects.push_back(background_2);
	m_gameObjects.push_back(gameOverText);
	m_gameObjects.push_back(button1);
	m_gameObjects.push_back(button2);

	std::cout << "entering PauseState\n";

	return true;
}

bool GameOverState::onExit()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}

	m_gameObjects.clear();

	TheTextureManager::Instance()->clearFromTextureMap("gameover_background");
	TheTextureManager::Instance()->clearFromTextureMap("player_background");
	TheTextureManager::Instance()->clearFromTextureMap("gameovertext");
	TheTextureManager::Instance()->clearFromTextureMap("mainbutton");
	TheTextureManager::Instance()->clearFromTextureMap("restartbutton");

	TheSoundManger::Instance()->stopPlaying();		//// 배경 음악 스톱

	std::cout << "exiting PauseState\n";

	return true;
}