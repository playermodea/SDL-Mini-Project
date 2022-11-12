#pragma once

#include "SDLGameObject.h"

class Background : public SDLGameObject {
public:
	Background(const LoaderParams* pParams) : SDLGameObject(pParams) {}
};

////// 원래 카메라 액션을 구현하려고 그와 연계된 백그라운드 클래스를 만들었지만 카메라 액션을 대략적으로 구현해 본 결과 본 게임과는 잘 맞지 않는다고 판단하여 카메라 액션은 삭제함
////// 백그라운드 클래스는 클래스 구별을 위해 남겨두었음