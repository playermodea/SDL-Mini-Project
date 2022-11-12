#pragma once

#include "GameObject.h"
#include "Vector2D.h"

class SDLGameObject : public GameObject {
	////// 각종 공용 함수 추가
public:
	SDLGameObject(const LoaderParams* pParams);
	virtual void draw();
	virtual void update();
	virtual void onBody() {}
	virtual void onLeg() {}
	virtual void isAttack() {}
	virtual ~SDLGameObject() {}
	virtual Vector2D getPosition() { return m_position; }
	virtual std::string getTextureID() { return m_textureID; }
	virtual int getWidth() { return m_width; }
	virtual int getHeight() { return m_height; }
	virtual int getState() { return m_state; }

	////// 각종 공용 변수 추가
protected:
	int m_width;
	int m_height;
	int m_currentRow;
	int m_currentFrame;
	int m_speed;
	int m_numFrames;
	int pivot_x;
	int pivot_y;
	int m_state;
	int prev_State;
	int start_Delay;
	int end_Delay;
	int attack_DelayTime;
	float m_angle;
	bool attacking;
	std::string m_textureID;

	Vector2D m_position;
	Vector2D m_velocity;
};

enum player_state {
	PLAYER_LEG = 0,
	PLAYER_BODY = 1,
	PLAYER_NONE, BACKGROUND = 2,
	PLAYER_PUNCH = 3,
	PLAYER_BAT = 4,
	PLAYER_BAT_ATTACK = 5,
};

enum enemy_state {
	ENEMY_NONE = 0,
	ENEMY_ATTACK = 1,
	ENEMY_DIE = 2
};