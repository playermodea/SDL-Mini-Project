#include "PauseState.h"
#include "MenuState.h"
#include "GameStateMachine.h"
#include "Game.h"
#include "MenuButton.h"
#include "TextureManager.h"
#include "Background.h"
#include <iostream>

PauseState* PauseState::s_plnstance = NULL;
const std::string PauseState::s_pauseID = "PAUSE";

////// 텍스쳐가 추가되고 onExit 함수를 다른 State 클래스랑 비슷하게 구성해준 것 외에 바뀐 부분 없음
void PauseState::s_pauseToMain()
{
	TheGame::Instance()->getStateMachine()->changeState(MenuState::Instance());
}

void PauseState::s_resumePlay()
{
	TheGame::Instance()->getStateMachine()->popState();
}

bool PauseState::onEnter()
{
	if (!TheTextureManager::Instance()->load("assets/background/pause_background.png", "pause_background", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/resume.png", "resumebutton", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/main.png", "mainbutton", TheGame::Instance()->getRenderer()))
		return false;

	GameObject* background = new Background(new LoaderParams(0, 0, 1920, 1080, "pause_background"));
	GameObject* button1 = new MenuButton(new LoaderParams(860, 250, 200, 80, "mainbutton"), s_pauseToMain);
	GameObject* button2 = new MenuButton(new LoaderParams(860, 750, 200, 80, "resumebutton"), s_resumePlay);

	m_gameObjects.push_back(background);
	m_gameObjects.push_back(button1);
	m_gameObjects.push_back(button2);

	std::cout << "entering PauseState\n";

	return true;
}

bool PauseState::onExit()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}

	m_gameObjects.clear();

	TheTextureManager::Instance()->clearFromTextureMap("pause_background");
	TheTextureManager::Instance()->clearFromTextureMap("resumebutton");
	TheTextureManager::Instance()->clearFromTextureMap("mainbutton");

	std::cout << "exiting MenuState\n";

	return true;
}