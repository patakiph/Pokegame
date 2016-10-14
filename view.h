#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;

void setPlayerCoordinateForView(float x, float y) {
	float tempX = x; float tempY = y;

	if (x < 320) tempX = 320;
	if (y < 320) tempY = 320;//верхнюю сторону
	if (y > 320) tempY = 320;//нижнюю стороню.для новой карты

	view.setCenter(tempX, tempY);
}