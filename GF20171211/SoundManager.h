#pragma once

#include "SDL_Mixer.h"

//// 사운드 매니저 클래스를 선언해 사운드 매니저를 한 곳에서 한번에 관리함 -> 싱글톤 사용
class SoundManager {
public:
	~SoundManager() {}

	void playBatswing();
	void playGameover();
	void playPlay();
	void playHit();
	void playMain();
	void playPickup();
	void playPunchswing();
	void stopPlaying();

	static SoundManager* Instance()
	{
		if (s_plnstance == 0) {
			s_plnstance = new SoundManager();
			return s_plnstance;
		}
		return s_plnstance;
	}

private:
	Mix_Music *play_menu_bgm;
	Mix_Music *play_play_bgm;
	Mix_Music *play_gameover_bgm;

	Mix_Chunk *pick_up_bgm;
	Mix_Chunk *hit_bgm;
	Mix_Chunk *punch_swing_bgm;
	Mix_Chunk *bat_swing_bgm;

	SoundManager();

	static SoundManager* s_plnstance;
};

typedef SoundManager TheSoundManger;