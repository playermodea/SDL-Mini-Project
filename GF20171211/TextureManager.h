#pragma once

#include "SDL.h"
#include "map"
#include "SDL_TTF.h"

class TextureManager {
public:
	~TextureManager() {}

	bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);

	void drawText(std::string text, int x, int y, int width, int height, TTF_Font* font, SDL_Color color, SDL_Renderer* pRenderer);

	void drawFrame(std::string id, int x, int y, int width, int height,
		int currentRow, int currentFrame, int pivot_x, int pivot_y, float angle,
		SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void clearFromTextureMap(std::string id) { SDL_DestroyTexture(m_textureMap[id]); m_textureMap.erase(id); }

	static TextureManager* Instance()
	{
		if (s_plnstance == 0) {
			s_plnstance = new TextureManager();
			return s_plnstance;
		}
		return s_plnstance;
	}

private:
	std::map<std::string, SDL_Texture*> m_textureMap;

	TextureManager() {}

	static TextureManager* s_plnstance;
};

typedef TextureManager TheTextureManager;