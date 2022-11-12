#pragma once

#include "LoaderParams.h"
#include "Vector2D.h"

class GameObject {
public:
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void onBody() = 0;
	virtual void onLeg() = 0;
	virtual void isAttack() = 0;

	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual int getState() = 0;

	virtual Vector2D getPosition() = 0;
	virtual std::string getTextureID() = 0;

	virtual ~GameObject() {}

protected:
	GameObject(const LoaderParams* pParams) {}
};