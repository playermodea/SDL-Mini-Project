#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Game.h"

SDLGameObject::SDLGameObject(const LoaderParams* pParams) : GameObject(pParams), m_position(pParams->getX(), pParams->getY()), m_velocity(0, 0)
{
	m_width = pParams->getWidth();		//// 공통 변수 초기화
	m_height = pParams->getHeight();
	m_textureID = pParams->getTextureID();
	m_currentRow = 0;
	m_currentFrame = 0;
	pivot_x = 0;
	pivot_y = 0;
	m_angle = 0;
	m_state = 0;
	prev_State = 0;
	start_Delay = 0;
	end_Delay = 0;
	attack_DelayTime = 0;
	m_numFrames = 1;
	m_speed = 2;
	attacking = true;
}

void SDLGameObject::draw()
{
	TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame, pivot_x, pivot_y, m_angle, TheGame::Instance()->getRenderer());
}

void SDLGameObject::update()
{
	m_position += m_velocity;
}