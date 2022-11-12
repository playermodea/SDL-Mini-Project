#pragma once

#include "SDL.h"
#include "vector"
#include "Vector2D.h"

enum mouse_buttons {
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
};

class InputHandler {
public:
	~InputHandler() {}

	static InputHandler* Instance() {
		if (s_plnstance == 0)
		{
			s_plnstance = new InputHandler();
		}

		return s_plnstance;
	}

	//// 키보드 입력처리 시작
	bool isKeyDown(SDL_Scancode key);
	void onKeyDown();
	void onKeyUp();
	//// 키보드 입력처리 끝

	void update();

	//// 마우스 입력처리 시작
	bool getMouseButtonState(int buttonNumber);
	Vector2D* getMousePosition();
	void onMouseMove(SDL_Event event);
	void onMouseButtonDown(SDL_Event event);
	void onMouseButtonUp(SDL_Event event);
	//// 마우스 입력처리 끝
private:
	InputHandler();
	static InputHandler* s_plnstance;
	const Uint8* m_keystates;

	std::vector<bool> m_mouseButtonStates;
	Vector2D* m_mousePosition;
};
typedef InputHandler TheInputHandler;