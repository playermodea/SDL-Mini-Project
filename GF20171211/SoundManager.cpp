#include "SoundManager.h"

SoundManager* SoundManager::s_plnstance = NULL;

//// 사운드매니저 로드
SoundManager::SoundManager()
{
	play_play_bgm = Mix_LoadMUS("assets/sound/snd_play.wav");
	play_menu_bgm = Mix_LoadMUS("assets/sound/snd_main.wav");
	play_gameover_bgm = Mix_LoadMUS("assets/sound/snd_gameover.wav");

	punch_swing_bgm = Mix_LoadWAV("assets/sound/snd_punch_swing.wav");
	hit_bgm = Mix_LoadWAV("assets/sound/snd_hit.wav");
	pick_up_bgm = Mix_LoadWAV("assets/sound/snd_pickup.wav");
	bat_swing_bgm = Mix_LoadWAV("assets/sound/snd_bat_swing.wav");
}

void SoundManager::playBatswing()
{
	Mix_PlayChannel(-1, bat_swing_bgm, 0);
}

void SoundManager::playPunchswing()
{
	Mix_PlayChannel(-1, punch_swing_bgm, 0);
}

void SoundManager::playHit()
{
	Mix_PlayChannel(-1, hit_bgm, 0);
}

void SoundManager::playPickup()
{
	Mix_PlayChannel(-1, pick_up_bgm, 0);
}

void SoundManager::playMain()
{
	Mix_PlayMusic(play_menu_bgm, -1);
}

void SoundManager::playPlay()
{
	Mix_PlayMusic(play_play_bgm, -1);
}

void SoundManager::playGameover()
{
	Mix_PlayMusic(play_gameover_bgm, -1);
}

void SoundManager::stopPlaying()
{
	Mix_HaltMusic();
}

//// SDL_Mixer 라이브러리 같은 경우는 제가 사용법을 잘 모르는건지 아니면 라이브러리 자체가 불안정해서 오류를 발생시키는 것인지는 잘 모르겠는데
//// 기존에 했던 객체에서마다 사운드를 로드하고 재생하고 해제하는 방법은 사운드를 해제하는 과정에서 자꾸 다른 클래스와 충돌을 일으켜 에러를 발생시키고
//// 사운드를 해제해도 메모리 사용량을 급증시키는 메모리 누수가 발생해서 SoundManager 클래스를 생성해서 싱글톤을 통해 한번에 관리하기로 했습니다.
//// 게임이 실행될 때 싱글톤이 만들어지고 게임에 필요한 사운드를 한번에 로드해서 필요한 상황때마다 상황에 맞는 효과음이나 배경음을 재생시킵니다.
//// 싱글톤을 사용하기 때문에 사운드가 단 한번씩만 로드가 되서 해제 과정을 넣지 않아도 따로 메모리 누수를 발생시키지는 않는다고 생각합니다.
//// 따라서 자꾸 에러를 발생시키는 해제과정은 불필요하다고 생각됐기 때문에 넣지 않았습니다.