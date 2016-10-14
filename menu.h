#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

void menu(RenderWindow & window) {
	Image menuIm1, menuIm2;
	menuIm1.loadFromFile("images/start_game.png");
	menuIm1.createMaskFromColor(Color::White);
	menuIm2.loadFromFile("images/quit.png");
	menuIm2.createMaskFromColor(Color::White);
	Texture menuTexture1, menuTexture2, menuBackground;
	menuTexture1.loadFromImage(menuIm1);
	menuTexture2.loadFromImage(menuIm2);
	menuBackground.loadFromFile("images/bg.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(160, 200);
	menu2.setPosition(0, 448);
	menuBg.setPosition(0, 0);

	//////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(160, 200, 316, 49).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(0, 448, 32, 32).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { window.close(); isMenu = false; }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.display();
	}
	////////////////////////////////////////////////////
}