#include "SDLGameState.h"
#include "GameObject.h"

//// GameState의 하위 클래스인 SDLGameState 클래스를 만들어 공통 함수를 정의해 줌으로써 GameState가 계속 추상클래스 인터페이스로 남을 수 있게 함
void SDLGameState::update()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->update();
}

void SDLGameState::render()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->draw();
}