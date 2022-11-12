#pragma once

#include "SDLGameObject.h"

class EnemySpawner : public SDLGameObject {
public:
	EnemySpawner(const LoaderParams* pParams);

	virtual void update();
	void onSpawn();

private:
	int spawn_Time;
	int start_Delay;
	int end_Delay;
	int spawn_Limit;
	int spawn_TimeClock;
};