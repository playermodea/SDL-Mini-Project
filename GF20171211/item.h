#pragma once

#include "SDLGameObject.h"

class item : public SDLGameObject {
public:
	item(const LoaderParams* pParams) : SDLGameObject(pParams) {}
};

////// 원래 총 등 기타 아이템들도 구현하려 했지만 시간 관계상 배트 아이템 하나만 구현하게 되었음
////// 아이템 클래스는 클래스 구별을 위해 남겨두었음