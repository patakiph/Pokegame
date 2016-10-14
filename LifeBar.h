#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;


class LifeBar {
public:
	Image image;
	Texture t;
	Sprite s;
	
	LifeBar()
	{
		image.loadFromFile("images/lifebar.png");
		image.createMaskFromColor(Color(255, 255, 255));
		t.loadFromImage(image);
		s.setTexture(t);
		s.setTextureRect(IntRect(0, 0, 33, 29));
	}

};