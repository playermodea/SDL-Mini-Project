#include "Enemy.h"
#include "PlayState.h"
#include "TextureManager.h"
#include "Game.h"
#include "HitBox.h"
#include "SoundManager.h"

Enemy::Enemy(const LoaderParams* pParams) :SDLGameObject(pParams)
{
	attack_DelayTime = 30;			//// 공격시 애니메이션 프레임 속도를 설정하기 위한 attack_DelayTime 변수 초기화
}

void Enemy::update()
{
	if (m_state != ENEMY_DIE)	//// Enemy 객체의 업데이트 함수는 죽음 상태가 아닐 경우에만 작동됨
	{
		handleInput();

		end_Delay = SDL_GetTicks();		//// end_Delay 변수를 매 프레임마다 현재 시간으로 초기화

		if (attacking)	//// 공격 가능 상태
			m_currentFrame = int(((SDL_GetTicks() / 100) % m_numFrames));	//// Enemy 객체가 공격상태가 아니라면 일반적으로 애니메이션 작동
		else	//// 공격 불가능 상태(공격 중일 때)
		{
			if (end_Delay - start_Delay >= attack_DelayTime)	//// end_Delay에서 start_Delay를 빼준 것이 attack_DelayTime보다 같거나 그 이상일 때 공격 애니메이션의 한프레임 움직임
			{
				m_currentFrame += 1;
				start_Delay = SDL_GetTicks();		//// end_Delay는 시간이 지날수록 커지기 때문에 이와 맞춰주기 위해 한프레임 움직였을 때 start_Delay를 현재 시간으로 초기화
			}

			if (m_currentFrame == m_numFrames - 1)	//// 공격 애니메이션이 한사이클을 전부 돌았을 때 히트박스 생성, Enemy객체의 다리와 몸통에서 전부 히트박스 생성할 필요가 없으므로 몸통에서만 히트박스 생성
			{										//// 플레이어 객체는 공격시 바로 히트박스 생성 -> 밸런스 위함
				if (m_textureID == "enemy")
					PlayState::Instance()->setObjects(new HitBox(new LoaderParams(m_position.getX(), m_position.getY(), 40, 35, "enemy_hit_box"), pivot_x, pivot_y, m_angle));

				m_state = prev_State;	//// 공격 애니메이션이 한 사이클을 다 돌았을 때 저장해둔 상태로 돌아감(공격바로 직전 상태)

				m_speed = 2;	//// 공격이 끝났기 때문에 이동을 위한 속도 설정

				attacking = true;	//// 공격가능 상태로 바뀜
			}
		}

		SDLGameObject::update();
	}
}

void Enemy::handleInput()
{
	//// 자연스러운 이동을 위해 플레이어의 방향을 바라보도록 각도 설정, 매 프레임마다 업데이트
	m_angle = atan2f(PlayState::Instance()->getObjects()[PLAYER_BODY]->getPosition().getY() - m_position.getY(), PlayState::Instance()->getObjects()[PLAYER_BODY]->getPosition().getX() - m_position.getX()) * 180 / M_PI;
	//// 플레이어와 현재 Enemy 객체의 위치를 계산해 나온 벡터값을 정규화 시켜 방향을 설정하고 그 방향으로 속도값만큼 움직임
	Vector2D unit_Vec = PlayState::Instance()->getObjects()[PLAYER_BODY]->getPosition() - m_position;
	unit_Vec.normalize();

	m_velocity = unit_Vec * m_speed;

	if (m_textureID == "enemy")		//// Enemy 객체의 몸통과 다리일 때 상태를 변경하기 위한 함수 호출 처음 생성시 부여되는 아이디로 몸통인지 다리인지 구분
		onBody();
	else if (m_textureID == "enemy_leg")
		onLeg();
}

void Enemy::onBody()
{
	switch (m_state)	//// m_state 값에 따라 Enemy 객체의 몸통 상태 변경 총 2단계로 구분 if - else if 문으로 구성해되지만 더 추가될 상태를 고려해 switch 문으로 작성
	{
	case ENEMY_NONE:
		m_currentRow = 0;
		m_width = 32;
		m_height = 32;
		m_numFrames = 12;
		break;
	case ENEMY_ATTACK:
		m_currentRow = 32;
		m_width = 40;
		m_height = 35;
		m_numFrames = 13;
		break;
	default:
		break;

	}
}

void Enemy::onLeg()
{
	switch (m_state)	//// m_state 값에 따라 Enemy 객체의 다리 상태 변경
	{
	case ENEMY_NONE:
		m_currentRow = 0;
		m_width = 32;
		m_height = 32;
		m_numFrames = 12;
		break;
	case ENEMY_ATTACK:
		m_currentRow = 32;
		m_width = 40;
		m_height = 30;
		m_numFrames = 13;
		break;
	default:
		break;
	}
}

///////////////////////////////////// onBody와 onLeg 함수에서 이미 변수의 값을 바꿔주고 있기 때문에 어쩔 수 없이 상수값 사용

void Enemy::isAttack()
{
	prev_State = m_state;	//// prev_State 변수에 현재의 상태 저장 (이 후 공격이 끝났을 때 이 변수를 통해 원래의 상태로 돌아감)
	m_state = ENEMY_ATTACK;		//// 현재의 상태를 m_state에 저장

	m_currentFrame = 0;		//// 보다 자연스러운 애니메이션을 위해 m_currentFrame을 0으로 설정함 (무조건 애니메이션이 첫 프레임부터 실행될 수 있도록)
	m_speed = 0;		///// 공격 상태이기 때문에 Enemy 객체의 공격 상태를 0으로 변경

	if (m_textureID == "enemy")		//// 사운드가 겹칠 수 있기 때문에 공격할 때 나는 효과음은 Enemy 객체의 몸통 부분에서만 실행되게 함
	{
		TheSoundManger::Instance()->playBatswing();
	}

	attacking = false;		//// 중복 공격을 막기 위해 attacking 변수를 false로 만들어 공격 불가 상태로 만듬 쉽게 말해 기본 상태에서 공격 중인 상태로 변경
}

void Enemy::isDie()
{
	m_state = ENEMY_DIE;	//// Enemy 객체의 상태 변경

	m_currentFrame = 0;
	m_angle -= 180;		//// 좀 더 자연스러운 상태로 쓰러지게끔 앵글 변경 -> 뒤로 넘어지게끔
	m_speed = 0;

	if (m_textureID == "enemy")		//// Enemy 객체의 몸통과 다리의 상태를 Die 상태로 바꿔줌. onBody와 onLeg 함수에서 설정안해준 이유
	{								//// -> 이미 죽은 상태이기 때문에 다른 객체와 상호작용하지 않음 따라서 업데이트 함수에서 상태를 계속 업데이트 할 필요 없기 때문
		m_currentRow = 72;
		m_width = 64;
		m_height = 41;
		TheSoundManger::Instance()->playHit();		//// 사운드가 겹칠 수 있기 때문에 Enemy 객체의 몸통에서만 소리가 나게끔 함
	}
	else if (m_textureID == "enemy_leg")
	{
		m_currentRow = 32;
		m_width = 40;
		m_height = 30;
	}
}