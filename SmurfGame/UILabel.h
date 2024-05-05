#ifndef UILABEL_H
#define UILABEL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "ECS.h"
#include "AssetManager.h"
#include "Game.h"

class UILabel : public Component {
public:
	UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color& colour): labelText(text), labelFont(font), textColour(colour) {

		this->position.x = xpos;
		this->position.y = ypos;


		this->setLabelText(this->labelText, this->labelFont);
	}
	~UILabel(){}

	void setLabelText(std::string text, std::string font) {
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->getFont(font), text.c_str(), this->textColour);
		this->labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(this->labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw() override {
		SDL_RenderCopy(Game::renderer, this->labelTexture, nullptr, &position);
	}


private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColour;
	SDL_Texture* labelTexture;
};

#endif // !UILABEL_H
