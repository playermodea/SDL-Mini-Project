#include "AnimatedGraphic.h"
#include "SDL.h"

AnimatedGraphic::AnimatedGraphic(const LoaderParams* pParams, int animSpeed) : SDLGameObject(pParams), m_animSpeed(animSpeed) {}

void AnimatedGraphic::update()
{
	m_numFrames = m_animSpeed;
	m_currentFrame = int(((SDL_GetTicks() / (1000 / m_animSpeed)) % m_numFrames));
}