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

	int labelID;

	UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color& colour, bool f) : labelText(text), labelFont(font), textColour(colour), fixed(f) {

		this->labelID = getLabelID();

		this->position.x = xpos;
		this->position.y = ypos;

		this->cameraPosition.x = xpos;
		this->cameraPosition.y = ypos;

		this->setLabelText(this->labelText, this->labelFont);
	}

	UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color& colour, bool f, float r) : labelText(text), labelFont(font), textColour(colour), fixed(f), radius(r) {

		this->labelID = getLabelID();

		this->position.x = xpos;
		this->position.y = ypos;

		this->cameraPosition.x = xpos;
		this->cameraPosition.y = ypos;

		this->setLabelText(this->labelText, this->labelFont);
	}

	~UILabel() {}

	void setLabelText(std::string text, std::string font) {
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->getFont(font), text.c_str(), this->textColour);
		this->labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(this->labelTexture, nullptr, nullptr, &cameraPosition.w, &cameraPosition.h);
	}

	void update() override {

		if (!this->fixed) {
			this->cameraPosition.x = this->position.x - Game::camera.x;
			this->cameraPosition.y = this->position.y - Game::camera.y;
		}
	}

	void draw() override {
		SDL_RenderCopy(Game::renderer, this->labelTexture, nullptr, &cameraPosition);
	}

	bool inRange(Vector2D vec) {
		float d = sqrt(pow(vec.x - this->position.x, 2) + pow(vec.y - this->position.y, 2));
		return d <= this->radius;
	}

private:

	SDL_Rect position;
	SDL_Rect cameraPosition;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColour;
	SDL_Texture* labelTexture;
	bool fixed = true;
	float radius;

	int getLabelID() {
		static int firstID = 0;
		return firstID++;
	}

};

#endif // !UILABEL_H