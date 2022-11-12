#pragma once

#include "SDLGameObject.h"

class HitBox : public SDLGameObject {
public:
	HitBox(const LoaderParams* pParams, int pivot_x, int pivot_y, float angle);

private:
	int pivotX, pivotY;
	float angle;
};