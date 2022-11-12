#include "Player.h"
#include "Game.h"
#include "PlayState.h"
#include "HitBox.h"
#include "InputHandler.h"
#include "item.h"
#include "SoundManager.h"

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	SDL_GetWindowSize(TheGame::Instance()->getWindow(), &window_w, &window_h);	//// 창 사이즈를 판별하기 위한 변수 초기화
	stamina = 200;			//// 스태미나 변수 초기화
	animation_Speed = 100;	//// 애니메이션 속도 조절을 위한 변수 초기화
	bat_Count = 0;			//// 배트 사용 횟수 변수 초기화
	hit_pivotX = 16;		//// 히트박스에 넘겨줄 피벗 변수 초기화
	hit_pivotY = 0;
	m_state = PLAYER_NONE;	//// 플레이어 초기 상태 초기화
	running = true;			//// 플레이어 달리기 상태 초기화
	bat_Mode = false;		//// 플레이어 초기 배트 모드 초기화
}

void Player::update()
{
	handleInput();

	//// 시간 측정 변수 계속 업데이트 해줘야 할 필요 있음
	end_Delay = SDL_GetTicks();

	//// 공격 중인지 아닌지 판별하고 애니메이션을 돌려주는 조건문
	if (attacking)
		m_currentFrame = int(((SDL_GetTicks() / animation_Speed) % m_numFrames));	//// attacking이 true라면 즉, 공격 상태가 아니라면 일반적으로 애니메이션을 돌려줌
	else        //// attacking이 false 라면 즉, 공격 중인 상태라면 애니메이션 메커니즘을 바꿔줌
	{
		if (m_state == PLAYER_PUNCH)	//// 플레이어 객체의 상태가 펀치 공격 상태라면 attack_DelayTime 변수를 100으로 바꿔줌으로써 애니메이션 속도를 배트 공격보다 느리게 설정
		{
			attack_DelayTime = 100;
		}
		else if (m_state == PLAYER_BAT_ATTACK)	//// 플레이어 객체의 상태가 배트 공격 상태라면 attack_DelayTime 변수를 30으로 바꿔줌으로써 애니메이션 속도를 펀치 속도보다 빠르게 설정
		{
			attack_DelayTime = 30;
		}

		if (end_Delay - start_Delay >= attack_DelayTime)	//// end_Delay에서 start_Delay를 빼준 것이 attack_DelayTime보다 같거나 그 이상일 때 공격 애니메이션의 한프레임 움직임
		{
			m_currentFrame += 1;
			start_Delay = SDL_GetTicks();	//// end_Delay는 시간이 지날수록 커지기 때문에 이와 맞춰주기 위해 한프레임 움직였을 때 start_Delay를 현재 시간으로 초기화
		}

		if (m_currentFrame == m_numFrames - 1)	//// 공격 애니메이션이 한 사이클을 전부 돌았다면 prev_State 변수를 통해 공격하기 전 상태로 돌아감(배트를 들고 있었다면 BAT 상태였다면 BAT 상태로, NONE 상태였다면 NONE상태로)
		{
			if (m_state == PLAYER_BAT_ATTACK)	//// 플레이어가 배트를 들고 있었다면 배트의 공격 횟수를 1 차감시킴
			{
				bat_Count -= 1;

				if (bat_Count == 0)		//// 배트 공격횟수를 전부 소진했을 경우 bat_Mode를 false로 바꿔 bat_Mode를 막아준 뒤 prev_State에 NONE 상태를 넣어 자동으로 맨 주먹 상태로 돌아가게 함
				{
					bat_Mode = false;
					prev_State = PLAYER_NONE;

					if (m_textureID == "player")	//// 랜덤 위치에 배트 아이템을 드랍시킴
						PlayState::Instance()->setObjects(new item(new LoaderParams(rand() % (window_w - 200) + 100, rand() % (window_h - 200) + 100, 70, 60, "bat")));

				}
			}

			m_state = prev_State;
			attacking = true;
		}
	}
	////// 플레이어 객체가 화면 밖으로 나가게 하지 못하게 하는 조건문
	if (m_position.getX() <= 0)
		m_position.setX(0);
	if (m_position.getY() <= 0)
		m_position.setY(0);
	if (m_width + m_position.getX() >= window_w)
		m_position.setX(window_w - m_width);
	if (m_height + m_position.getY() >= window_h)
		m_position.setY(window_h - m_height);

	SDLGameObject::update();
}

void Player::handleInput()
{
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
		m_velocity.setX(m_speed);
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
		m_velocity.setX(-m_speed);
	else
		m_velocity.setX(0);

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
		m_velocity.setY(-m_speed);
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
		m_velocity.setY(m_speed);
	else
		m_velocity.setY(0);

	////// 플레이어 객체의 몸통과 다리에 각기 다른 각도로 움직이게 각도 설정 몸통은 마우스 커서를 바라보고 다리는 키보드 입력을 통해 앵글을 바꿔줌
	////// -> 보다 자연스러운 움직임을 위함 만약 몸통과 다리 전부 마우스 커서를 바라보도록 각도가 설정된다면 마우스 커서의 반대방향으로 이동시 부자연스럽게 움직이게 됨
	if (m_textureID == "player")
	{
		m_angle = atan2f(TheInputHandler::Instance()->getMousePosition()->getY() - m_position.getY(), TheInputHandler::Instance()->getMousePosition()->getX() - m_position.getX()) * 180 / M_PI;
	}
	else if (m_textureID == "player_leg")
	{
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D) && TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
			m_angle = 315;
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W) && TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
			m_angle = 225;
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A) && TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
			m_angle = 135;
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S) && TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
			m_angle = 45;
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
			m_angle = 0;
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
			m_angle = 270;
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
			m_angle = 180;
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
			m_angle = 90;
	}

	///// running 변수가 true라면 달릴 수 있게 해줌 -> 스피드를 3으로 바꾸고 애니메이션 속도 변수를 50으로 바꿔 애니메이션 속도를 빠르게 변경함 스태미너가 0이하로 내려가면 running 변수 false로 초기화
	if (running == true && stamina > 0 && TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LSHIFT))
	{
		m_speed = 3;
		animation_Speed = 50;
		stamina -= 1;

		if (stamina <= 0)
			running = false;
	}
	else  ////// 달릴 수 없는 상태 즉, running 변수가 false 상태라면 매 프레임마다 stamina 변수를 조금씩 증가시켜주고 stamina 변수가 200이상이 된다면 증가를 멈추고 running 변수를 true로 초기화
	{	  ///// 이 때 다시 걷기 상태가 되므로 스피드는 2로 애니메이션 속도 변수를 다시 100으로 초기화 시켜 애니메이션 속도를 느리게 해줌
		m_speed = 2;
		animation_Speed = 100;

		if (stamina < 200)
			stamina += 1;
		else if (stamina >= 200)
			running = true;
	}


	////// 공격 가능 상태에서 마우스 왼쪽 클릭시 어택 함수 실행
	if (TheInputHandler::Instance()->getMouseButtonState(LEFT) && attacking == true)
		isAttack();

	////// 1번 2번으로 공격 모드를 바꿔줄 수 있음. 단 배트 공격은 bat_Mode가 true여야함 즉, 배트 아이템을 먹은 상태인 경우에만 바꿔질 수 있음
	if (attacking)
	{
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_1))
			m_state = PLAYER_NONE;
		else if (bat_Mode == true && TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_2))
			m_state = PLAYER_BAT;
	}

	////// 아이디를 통해 플레이어 객체의 몸통과 다리를 구분하고 이에 따라 각기 다르게 상태를 변경함
	if (m_textureID == "player")
		onBody();
	else if (m_textureID == "player_leg")
		onLeg();
}

void Player::onBody()
{
	switch (m_state)
	{
	case PLAYER_NONE:
		m_currentRow = 0;
		m_width = 32;
		m_height = 32;
		m_numFrames = 14;
		break;
	case PLAYER_PUNCH:
		m_currentRow = 32;
		m_width = 48;
		m_height = 32;
		m_numFrames = 4;
		break;
	case PLAYER_BAT:
		m_currentRow = 64;
		m_width = 32;
		m_height = 47;
		m_numFrames = 14;
		break;
	case PLAYER_BAT_ATTACK:
		m_currentRow = 111;
		m_width = 40;
		m_height = 40;
		m_numFrames = 9;
		break;
	default:
		break;
	}
	if (m_velocity.getX() == 0 && m_velocity.getY() == 0 && attacking)	//// 정지 상태이고 공격 중이 아닐 때 애니메이션이 작동되지 않도록 함
	{
		m_currentFrame = 0;
		m_numFrames = 1;
	}
}

void Player::onLeg()
{
	switch (m_state)
	{
	case PLAYER_NONE:
		m_currentRow = 0;
		m_numFrames = 13;
		m_position.setX(PlayState::Instance()->getObjects()[PLAYER_BODY]->getPosition().getX());	//// 플레이어 몸통의 좌표값을 따라감
		m_position.setY(PlayState::Instance()->getObjects()[PLAYER_BODY]->getPosition().getY());
		break;
	case PLAYER_BAT:
		if (m_currentFrame >= 6)
			m_currentFrame = 0;
		m_currentRow = 32;
		m_numFrames = 6;
		m_position.setX(PlayState::Instance()->getObjects()[PLAYER_BODY]->getPosition().getX() - 1);	//// 마찬가지로 플레이어 몸통의 좌표값을 따라감 하지만 배트 상태일 때 텍스쳐의 크기와 중심점이
		m_position.setY(PlayState::Instance()->getObjects()[PLAYER_BODY]->getPosition().getY() + 5);	//// NONE 상태와 조금 다르기 때문에 이를 위해 위치를 조정해줌
		break;
	case PLAYER_PUNCH:
		m_numFrames = 4;	//// m_numFrames 변수를 바꿔주는 이유는 몸통 쪽 애니메이션과 프레임을 맞춰주기 위함임 (펀치 공격 애니메이션은 4프레임, 배트 공격 애니메이션은 9프레임)
		break;
	case PLAYER_BAT_ATTACK: //// m_numFrames 변수를 바꿔주는 이유는 몸통 쪽 애니메이션과 프레임을 맞춰주기 위함임 (펀치 공격 애니메이션은 4프레임, 배트 공격 애니메이션은 9프레임)
		m_numFrames = 9;
		break;
	default:
		break;
	}
	if(!attacking || (m_velocity.getX() == 0 && m_velocity.getY() == 0))	//// 속도를 통해 플레이어가 서있는 상태인지 공격 중인 상태인지 판별하고 만약 서있거나 공격 중이라면 너비와 높이를
	{																		//// 0으로 바꿔 스프라이트가 보이지 않도록 함
		m_width = 0;
		m_height = 0;
	}
	else     ////// 움직이거나 공격 중인 상태가 아니라면 너비와 높이를 다시 원상태로 돌려줌
	{
		m_width = 32;
		m_height = 32;
	}
}

///////////////////////////////////// onBody와 onLeg 함수에서 이미 변수의 값을 바꿔주고 있기 때문에 어쩔 수 없이 상수값 사용 (Enemy 클래스와 동일)

void Player::isAttack()
{
	prev_State = m_state;	//// prev_State에 현재 상태 저장 (공격이 끝나고 공격 이전 상태로 돌아가기 위함)

	switch (m_state)	//// 펀치와 배트 공격을 구분하고 이에 따라 다른 히트박스를 생성시켜줌(배트가 범위가 조금 더 넓음) 그리고 두 텍스쳐의 크기와 중심점이 조금씩 다르기 때문에 hit_pivot 변수값을
	{					//// 통해 이를 조정해줌
	case PLAYER_NONE:	//// Enemy 객체와 마찬가지로 몸통에서만 소리와 히트박스가 생성되도록 함 Enmey 객체는 이와 다르게 공격 애니메이션이 모두 돌아가야 히트박스 생성 -> 밸런스 위함
		if (m_textureID == "player")
		{
			hit_pivotY = 16;
			PlayState::Instance()->setObjects(new HitBox(new LoaderParams(m_position.getX(), m_position.getY(), 50, 32, "player_punch"), hit_pivotX, hit_pivotY, m_angle));
			TheSoundManger::Instance()->playPunchswing();
		}
		m_state = PLAYER_PUNCH;
		m_currentFrame = 0;
		break;
	case PLAYER_BAT:
		if (m_textureID == "player")
		{
			hit_pivotY = 22;
			PlayState::Instance()->setObjects(new HitBox(new LoaderParams(m_position.getX(), m_position.getY(), 50, 44, "player_bat_attack"), hit_pivotX, hit_pivotY, m_angle));
			TheSoundManger::Instance()->playBatswing();
		}
		m_state = PLAYER_BAT_ATTACK;
		m_currentFrame = 0;
		break;
	default:
		break;
	}
	attacking = false;
}