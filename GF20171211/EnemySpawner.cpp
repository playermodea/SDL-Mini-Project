#include "EnemySpawner.h"
#include "Enemy.h"
#include "SDL.h"
#include "PlayState.h"

EnemySpawner::EnemySpawner(const LoaderParams* pParams) :SDLGameObject(pParams)
{
	spawn_Time = 0;		//// 각 변수 초기화
	start_Delay = 0;
	end_Delay = 0;
	spawn_Limit = 30;
	spawn_TimeClock = 6;
}

void EnemySpawner::update()
{
	end_Delay = SDL_GetTicks();		//// 일정시간마다 Enemy 객체를 생성하기 위해 end_Delay 변수를 매 프레임마다 현재 시간으로 초기화

	if (end_Delay - start_Delay >= spawn_Time * 1000 && PlayState::Instance()->getObjects().size() <= spawn_Limit)		//// spawn_Time에 따라 Enemy 객체 생성
		onSpawn();																										//// 단, PlayState의 게임 벡터 내 오브젝트 수가 spawn_Limit 이하일 때만 생성
}

void EnemySpawner::onSpawn()
{
	spawn_Time = rand() % spawn_TimeClock + 1;	//// 한번 Enemy 객체가 생성될 때마다 spawn_TimeClock 초 내에 있는 랜덤한 시간으로 스폰시간 변경 -> 이 전 스폰타임이 5초였지만 다음은 3초 후 스폰될 수도 있음

	PlayState::Instance()->setObjects(new Enemy(new LoaderParams(m_position.getX(), m_position.getY(), 32, 32, "enemy_leg")));
	PlayState::Instance()->setObjects(new Enemy(new LoaderParams(m_position.getX(), m_position.getY(), 32, 32, "enemy")));

	start_Delay = SDL_GetTicks();		//// end_Delay는 시간이 지날수록 커지기 때문에 이와 맞춰주기 위해 한프레임 움직였을 때 start_Delay를 현재 시간으로 초기화
}