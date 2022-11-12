#pragma once

#include "GameState.h"
////// GameState는 최상위 추상 클래스로 남겨두고 하위 SDLGameState를 선언해 공용 함수 정의 Play,Menu,Pause,Gameover State는 SDLGameState를 상속 받음
class SDLGameState : public GameState {
public:
	virtual void update();
	virtual void render();
	virtual bool onEnter() { return true; }
	virtual bool onExit() { return true; }

	virtual std::vector<GameObject*> getObjects() { return m_gameObjects; }
	virtual void setObjects(GameObject* gameObjects) { m_gameObjects.push_back(gameObjects); }
	virtual std::string getStateID() const = 0;
};