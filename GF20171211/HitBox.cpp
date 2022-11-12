#include "HitBox.h"

HitBox::HitBox(const LoaderParams* pParams, int pivotX, int pivotY, float angle) :SDLGameObject(pParams), pivotX(pivotX), pivotY(pivotY), angle(angle)
{
	pivot_x = pivotX;	//// 각도와 어딜 중심으로 회전할지 판별하는 pivot 값을 생성자에서 값을 받아 초기화 시켜줌
	pivot_y = pivotY;
	m_angle = angle;
}