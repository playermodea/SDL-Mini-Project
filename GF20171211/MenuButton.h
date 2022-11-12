#include "SDLGameObject.h"

class LoaderParams;

enum button_state {
	MOUSE_OUT = 0,
	MOUSE_OVER = 1,
	CLICKED = 2
};

class MenuButton : public SDLGameObject {
public:
	MenuButton(const LoaderParams* pParams, void(*callback)());
	virtual void update();

private:
	bool m_bReleased;
	void(*m_callback)();
};
