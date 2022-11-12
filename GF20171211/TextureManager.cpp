#include "TextureManager.h"
#include "SDL_image.h"
#include "Game.h"

TextureManager* TextureManager::s_plnstance = 0;

bool TextureManager::load(std::string fileName, std::string id, SDL_Renderer* pRenderer)
{
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

	if (pTempSurface == 0) {
		false;
	}
	SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);
	if (pTexture != 0) {
		m_textureMap[id] = pTexture;
		return true;
	}

	return false;
}

//// 텍스트를 랜더링 해주는 함수
void TextureManager::drawText(std::string text, int x, int y, int width, int height, TTF_Font* font, SDL_Color color, SDL_Renderer* pRenderer)
{
	SDL_Rect destRect = { x,y,width,height };	//// 대상 상자값 초기화
	SDL_Surface* load_text = TTF_RenderText_Solid(font, text.c_str(), color);	//// 텍스트를 surface에 넣어줌
	SDL_Texture* Message = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), load_text);		//// surface를 통해 텍스트를 텍스쳐로 변환
	SDL_FreeSurface(load_text);		//// surface 해제

	SDL_RenderCopy(pRenderer, Message, NULL, &destRect);	//// 텍스트 랜더링

	SDL_DestroyTexture(Message);	//// 텍스쳐 해제
}

//// pivot, angle 값이 추가된 랜더링 함수
void TextureManager::drawFrame(std::string id, int x, int y, int width, int height,
	int currentRow, int currentFrame,
	int pivot_x, int pivot_y, float angle, SDL_Renderer* pRenderer, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_Point pivot = { pivot_x, pivot_y };

	srcRect.x = width * currentFrame;
	srcRect.y = currentRow;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	//// pivot 값이 0이라면 pivot 값에 NULL 값을 넣어줌으로써 이미지의 중앙이 중심점이 되도록 함
	if (pivot_x == 0 && pivot_y == 0)
		SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, angle, 0, flip);
	else
		SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, angle, &pivot, flip);
}