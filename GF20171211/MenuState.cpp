#include "MenuState.h"
#include "Game.h"
#include "PlayState.h"
#include "MenuButton.h"
#include "TextureManager.h"
#include "GameStateMachine.h"
#include "Background.h"
#include "SoundManager.h"
#include <iostream>

MenuState* MenuState::s_plnstance = NULL;
const std::string MenuState::s_menuID = "MENU";

//////// 배경음악과 텍스쳐가 추가된 것 이외에 바뀐 부분 없음
bool MenuState::onEnter()
{
	TheSoundManger::Instance()->playMain();		////// 배경 음악 재생

	if (!TheTextureManager::Instance()->load("assets/background/menu_background.png", "menu_background", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/button.png", "playbutton", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/exit.png", "exitbutton", TheGame::Instance()->getRenderer()))
		return false;

	GameObject* background = new Background(new LoaderParams(0, 0, 1920, 1080, "menu_background"));
	GameObject* button1 = new MenuButton(new LoaderParams(760, 420, 400, 100, "playbutton"), s_menuToPlay);
	GameObject* button2 = new MenuButton(new LoaderParams(760, 620, 400, 100, "exitbutton"), s_exitFromMenu);

	m_gameObjects.push_back(background);
	m_gameObjects.push_back(button1);
	m_gameObjects.push_back(button2);

	std::cout << "entering MenuState\n";

	return true;
}

bool MenuState::onExit()
{
	TheSoundManger::Instance()->stopPlaying();

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}

	m_gameObjects.clear();

	TheTextureManager::Instance()->clearFromTextureMap("menu_background");
	TheTextureManager::Instance()->clearFromTextureMap("playbutton");
	TheTextureManager::Instance()->clearFromTextureMap("exitbutton");

	TheSoundManger::Instance()->stopPlaying();		//// 배경 음악 스톱

	std::cout << "exiting MenuState\n";

	return true;
}

void MenuState::s_menuToPlay()
{
	std::cout << "Play button clicked\n";
	TheGame::Instance()->getStateMachine()->changeState(PlayState::Instance());
}

void MenuState::s_exitFromMenu()
{
	std::cout << "Exit button clicked\n";
	TheGame::Instance()->quit();
}