#include "PlayState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "GameStateMachine.h"
#include "Game.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "EnemySpawner.h"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"
#include "item.h"
#include "SoundManager.h"
#include <iostream>

PlayState* PlayState::s_plnstance = NULL;
const std::string PlayState::s_playID = "PLAY";

void PlayState::update()
{
	end_Delay = SDL_GetTicks();

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		TheGame::Instance()->getStateMachine()->pauseState(PauseState::Instance());

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->update();	//// 각 객체의 업데이트 함수 불러옴

		//// 플레이어 객체가 배트 아이템과 충돌한다면 플레이어 객체의 bat_Mode를 활성화 시켜주고 배트 아이템 삭제
		if (m_gameObjects[i]->getTextureID() == "bat" && checkCollision(dynamic_cast<SDLGameObject*>(m_gameObjects[PLAYER_BODY]), dynamic_cast<SDLGameObject*>(m_gameObjects[i])))
		{
			dynamic_cast<Player*>(m_gameObjects[PLAYER_LEG])->setMode();
			dynamic_cast<Player*>(m_gameObjects[PLAYER_BODY])->setMode();
			TheSoundManger::Instance()->playPickup();
			delete m_gameObjects[i];
			m_gameObjects.erase(m_gameObjects.begin() + i);
			i--;
		}

		//// Enemy 객체가 플레이어 객체와 충돌한다면 Enemy 객체의 공격 모드를 활성화 시켜줌 (enemy_leg과 충돌체크한 이유는 다리가 몸통보다 먼저 벡터에 들어오기 때문에 편의상 그런 것)
		if (m_gameObjects[i]->getTextureID() == "enemy_leg" && m_gameObjects[i]->getState() == ENEMY_NONE && checkCollision(dynamic_cast<SDLGameObject*>(m_gameObjects[PLAYER_BODY]), dynamic_cast<SDLGameObject*>(m_gameObjects[i])))
		{
			m_gameObjects[i]->isAttack();
			m_gameObjects[i + 1]->isAttack();
		}

		//// 플레이어의 히트박스가 Enemy 객체와 충돌할 경우 Enemy 객체 상태가 Die 상태로 바뀌게 함
		//// 이 때 히트박스 하나당 한 객체만 공격할 수 있도록 한번 충돌체크에 성공할 경우 break를 통해 for문을 빠져나오도록 함
		for (int j = 7; j < m_gameObjects.size(); j++)	//// 6번 벡터까지는 고정값이기 때문에 7부터 시작
		{
			if (m_gameObjects[i]->getTextureID() == "player_punch" || m_gameObjects[i]->getTextureID() == "player_bat_attack")
			{
				if (m_gameObjects[j]->getState() != ENEMY_DIE && m_gameObjects[j]->getTextureID() == "enemy_leg" && checkCollision(dynamic_cast<SDLGameObject*>(m_gameObjects[i]), dynamic_cast<SDLGameObject*>(m_gameObjects[j])))
				{
					dynamic_cast<Enemy*>(m_gameObjects[j])->isDie();
					dynamic_cast<Enemy*>(m_gameObjects[j + 1])->isDie();
					kill_score += 1;
					break;
				}
			}
			else
				break;
		}

		//// Enemy 객체의 히트박스가 플레이어와 충돌할 경우 게임오버 상태로 변환
		if (m_gameObjects[i]->getTextureID() == "enemy_hit_box" && checkCollision(dynamic_cast<SDLGameObject*>(m_gameObjects[PLAYER_BODY]), dynamic_cast<SDLGameObject*>(m_gameObjects[i])))
		{
			TheGame::Instance()->getStateMachine()->changeState(GameOverState::Instance());
			break;
		}

		//// 히트박스를 바로 삭제해 줌 -> 생성과 삭제가 한 프레임에 전부 이루어지기 때문에 굳이 보이지 않는 이미지로 히트박스를 구성하지 않아도 우리 눈에는 보이지 않음
		if (m_gameObjects[i]->getTextureID() == "player_punch" || m_gameObjects[i]->getTextureID() == "player_bat_attack" || m_gameObjects[i]->getTextureID() == "enemy_hit_box")
		{
			delete m_gameObjects[i];
			m_gameObjects.erase(m_gameObjects.begin() + i);
			i--;
		}
	}

	//// 일정시간마다 벡터를 검사하여 죽은 Enemy 객체를 삭제해줌
	if (end_Delay - start_Delay >= clean_Time)
	{
		for (int i = 7; i < m_gameObjects.size(); i++)
		{
			if ((m_gameObjects[i]->getTextureID() == "enemy" || m_gameObjects[i]->getTextureID() == "enemy_leg") && m_gameObjects[i]->getState() == ENEMY_DIE)
			{
				delete m_gameObjects[i];
				m_gameObjects.erase(m_gameObjects.begin() + i);
				i--;
			}
		}

		start_Delay = SDL_GetTicks();
	}
}

void PlayState::render()
{
	std::string str_1 = std::string("KILL COUNT: ") + std::to_string(kill_score);
	std::string str_2 = std::to_string(dynamic_cast<Player*>(m_gameObjects[PLAYER_BODY])->getBatCount());

	//// 백그라운드를 맨 처음 랜더링 해주고 그 위에 킬 카운트 텍스트, Enemy Spawner, Enemy, Player 객체, 배트 공격 횟수 텍스트를 랜더링 해줌 -> 자연스러운 랜더링을 하기 위함
	m_gameObjects[BACKGROUND]->draw();
	TheTextureManager::Instance()->drawText(str_1, 710, 415, 500, 250, TheGame::Instance()->getFont(), TheGame::Instance()->getColor(), TheGame::Instance()->getRenderer());
	for (int i = 3; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->draw();
	m_gameObjects[PLAYER_LEG]->draw();
	m_gameObjects[PLAYER_BODY]->draw();
	TheTextureManager::Instance()->drawText(str_2, TheInputHandler::Instance()->getMousePosition()->getX() + 16 , TheInputHandler::Instance()->getMousePosition()->getY(), 32, 32, TheGame::Instance()->getFont(), TheGame::Instance()->getColor(), TheGame::Instance()->getRenderer());
}

////// 새로운 텍스쳐 추가 및 배경 음악 재생
bool PlayState::onEnter()
{
	kill_score = 0;
	start_Delay = 0;
	clean_Time = 10000;

	TheSoundManger::Instance()->playPlay();

	if (!TheTextureManager::Instance()->load("assets/background/play_background.png", "play_background", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/player/spr_player.png", "player", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/player/spr_player_leg.png", "player_leg", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/player/spr_player_punch_hit_box.png", "player_punch", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/player/spr_player_bat_hit_box.png", "player_bat_attack", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/enemy/spr_enemy.png", "enemy", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/enemy/spr_enemy_leg.png", "enemy_leg", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/enemy/spr_enemy_spawner.png", "enemy_spawner", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/enemy/spr_enemy_attack_hit_box.png", "enemy_hit_box", TheGame::Instance()->getRenderer()))
		return false;
	if (!TheTextureManager::Instance()->load("assets/item/spr_bat.png", "bat", TheGame::Instance()->getRenderer()))
		return false;

	GameObject* background = new Background(new LoaderParams(0, 0, 1920, 1080, "play_background"));
	GameObject* player = new Player(new LoaderParams(900, 500, 32, 32, "player"));
	GameObject* player_leg = new Player(new LoaderParams(900, 500, 32, 32, "player_leg"));
	GameObject* enemySP_1 = new EnemySpawner(new LoaderParams(10, 10, 32, 32, "enemy_spawner"));
	GameObject* enemySP_2 = new EnemySpawner(new LoaderParams(1878, 10, 32, 32, "enemy_spawner"));
	GameObject* enemySP_3 = new EnemySpawner(new LoaderParams(10, 1038, 32, 32, "enemy_spawner"));
	GameObject* enemySP_4 = new EnemySpawner(new LoaderParams(1878, 1038, 32, 32, "enemy_spawner"));
	GameObject* bat = new item(new LoaderParams(rand() % 1500 + 1, rand() % 900 + 1, 70, 60, "bat"));

	m_gameObjects.push_back(player_leg);
	m_gameObjects.push_back(player);
	m_gameObjects.push_back(background);
	m_gameObjects.push_back(enemySP_1);
	m_gameObjects.push_back(enemySP_2);
	m_gameObjects.push_back(enemySP_3);
	m_gameObjects.push_back(enemySP_4);
	m_gameObjects.push_back(bat);

	std::cout << "entering PlayState\n";

	return true;
}

bool PlayState::onExit()
{
	TheSoundManger::Instance()->stopPlaying();

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}

	m_gameObjects.clear();

	TheTextureManager::Instance()->clearFromTextureMap("play_background");
	TheTextureManager::Instance()->clearFromTextureMap("player");
	TheTextureManager::Instance()->clearFromTextureMap("player_leg");
	TheTextureManager::Instance()->clearFromTextureMap("player_punch");
	TheTextureManager::Instance()->clearFromTextureMap("player_bat_attack");
	TheTextureManager::Instance()->clearFromTextureMap("enemy");
	TheTextureManager::Instance()->clearFromTextureMap("enemy_leg");
	TheTextureManager::Instance()->clearFromTextureMap("enemy_spawner");
	TheTextureManager::Instance()->clearFromTextureMap("enemy_hit_box");
	TheTextureManager::Instance()->clearFromTextureMap("bat");

	//// 현재 기록이 최고 기록보다 높다면 최고 기록에 현재 기록값을 넣어줌
	TheGame::Instance()->setCurrentrecord(kill_score);
	if (kill_score > TheGame::Instance()->getBestrecord())
		TheGame::Instance()->setBestrecord(kill_score);

	std::cout << "exiting PlayState\n";

	return true;
}

bool PlayState::checkCollision(SDLGameObject* p1, SDLGameObject* p2)
{
	int leftA, leftB, rightA, rightB;
	int topA, topB, bottomA, bottomB;

	leftA = p1->getPosition().getX();
	rightA = p1->getPosition().getX() + p1->getWidth();
	topA = p1->getPosition().getY();
	bottomA = p1->getPosition().getY() + p1->getHeight();

	leftB = p2->getPosition().getX();
	rightB = p2->getPosition().getX() + p2->getWidth();
	topB = p2->getPosition().getY();
	bottomB = p2->getPosition().getY() + p2->getHeight();

	if (bottomA <= topB) return false;
	if (topA >= bottomB) return false;
	if (rightA <= leftB) return false;
	if (leftA >= rightB) return false;

	return true;
}