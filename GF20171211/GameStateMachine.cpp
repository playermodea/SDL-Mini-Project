#include "GameStateMachine.h"

void GameStateMachine::popState()
{
	m_currentState->onExit();
	m_currentState = m_prevState;
}

void GameStateMachine::pauseState(GameState* pState)	//// pause 상태로 게임이 돌입했을 때 이 전에 플레이하던 PlayState의 상태를 저장하기 위한 pauseState 함수 추가
{
	if (m_currentState != NULL)
	{
		m_prevState = m_currentState;
	}

	m_currentState = pState;
	m_currentState->onEnter();
}

void GameStateMachine::changeState(GameState* pState)
{
	if (m_prevState != NULL)
	{
		m_prevState->onExit();
	}

	if (pState != NULL)
	{
		if (m_currentState != NULL)
		{
			m_currentState->onExit();
		}

		m_currentState = pState;
		m_currentState->onEnter();
	}
}

void GameStateMachine::update()
{
	if (m_currentState != NULL)
		m_currentState->update();
}

void GameStateMachine::render()
{
	if (m_currentState != NULL)
		m_currentState->render();
}