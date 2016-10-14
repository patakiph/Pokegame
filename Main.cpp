#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
//#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "level.hpp"
#include "LifeBar.h"
#include "menu.h"
#include <vector>
#include <list>


using namespace sf;
////////////////////////////////////Общий класс-родитель//////////////////////////
class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 90; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
	virtual void update(float time) = 0;
};
////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;
	float currentFrame;
	Player(Image &image, String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		currentFrame = 0;
		playerScore = 0; state = stay; obj = lev.GetAllObjects();//инициализируем.получаем все объекты для взаимодействия персонажа с картой
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(0, 910, w, h));
		}
	}

	void control() {
		state = stay;
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				state = left; speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; speed = 0.1;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {
				state = jump; dy = -0.6; onGround = false;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = down;
			}
		}
		
	}



	void checkCollisionWithMap(float Dx, float Dy)
	{
		

		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy>0) { 
						y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time)
	{
		if (currentFrame >= 4)
			currentFrame -= 4;
		control();
		switch (state)
		{
		case right:dx = speed; break;
		case left:dx = -speed; break;
		case up: break;
		case down: dx = 0; break;
		case stay: dx = 0; break;
		}
		x += dx*time;
		checkCollisionWithMap(dx, 0);
		y += dy*time;
		if (y > 600) {
			life = false;
			y = 500;
		}
		checkCollisionWithMap(0, dy);
		animation(time);
		sprite.setPosition(x +w/2, y + h/2);
		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		setPlayerCoordinateForView(x, y);
		if (life) { setPlayerCoordinateForView(x, y); }
		dy = dy + 0.0015*time;
	}
	void animation(float time) {
		if (state == right) {
			sprite.setTextureRect(IntRect((int)currentFrame * 92, 910, 92, 42));
			currentFrame += time*0.005;
	
		}
		if (state == left) {
			sprite.setTextureRect(IntRect((int)currentFrame * 92 + 92, 910, -92, 42));
			currentFrame += time*0.005;
			if (currentFrame >= 4) currentFrame -= 4;
		}
		if (state == stay) {
			sprite.setTextureRect(IntRect((int)currentFrame * 92, 428, 92, 44));
			currentFrame += time*0.005;
			if (currentFrame >= 4)
				currentFrame -= 4;
		}
	}
};

////////////////////////////////////////////////////КЛАСС ВРАГА////////////////////////

class Enemy :public Entity {
public:
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(300, 0, w, h));
			dx = 0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		

		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid"){//если встретили препятствие (объект с именем solid)
				if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx>0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
				if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {
			moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
			checkCollisionWithMap(dx, 0);
			x += dx*time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};
////////////////////////////////////////////////////MAIN///////////////////////////////////////////////
bool startGame()
{
	RenderWindow window(VideoMode(640, 480), "Pokegame");
	menu(window);
	view.reset(FloatRect(0, 0, 640, 640));
	Font font;
	font.loadFromFile("ARCADECLASSIC.ttf");
	Text text("", font, 20);
	text.setColor(Color::Black);

	text.setString("GAME OVER");
	Music music;//создаем объект музыки
	music.openFromFile("Super_Mario_Bros._theme.ogg");//загружаем файл
													  //воспроизводим музыку

	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("pokemap.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	std::list <Entity *> entities;
	std::list <Entity*>::iterator it;
	std::vector<Object> e = lvl.GetObjects("easyEnemy");
	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/pokeball.png");
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 100, 100));

	Image heroImage;
	heroImage.loadFromFile("images/pikachu_sprite.png");



	Object player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
											//	Object easyEnemyObject = lvl.GetObject("easyEnemy");//объект легкого врага на нашей карте.задаем координаты игроку в начале при помощи него

	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 80, 42);//передаем координаты прямоугольника player из карты в координаты нашего игрока
																				   //	Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 100, 100);//передаем координаты прямоугольника easyEnemy из карты в координаты нашего врага

	LifeBar life;

	Clock clock;
	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//если таб, то перезагружаем игру
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//если эскейп, то выходим из игры
		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 900;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (music.getStatus() == false)
			music.play();
		p.update(time);
		for (it = entities.begin(); it != entities.end();) {
			(*it)->update(time);
			if ((*it)->life == false) it = entities.erase(it);
			else
				it++;
		}
		for (it = entities.begin(); it != entities.end(); it++) {
			if ((*it)->getRect().intersects(p.getRect())) {
				if ((*it)->name == "EasyEnemy") {
					if (p.dy > 0 && p.onGround == false) {
						(*it)->dx = 0;
						(*it)->health = 0;
						p.dy = -0.2;
					}
					else {
						p.health -= 3;
					}
				}
			}
		}
		//		easyEnemy.update(time);
		window.setView(view);
		window.clear(Color(77, 83, 140));
		lvl.Draw(window);
		//		window.draw(easyEnemy.sprite);
		for (it = entities.begin(); it != entities.end(); it++)
			window.draw((*it)->sprite);
		window.draw(p.sprite);
		for (int i = 0; i < p.health / 30; i++) {
			window.draw(life.s);
			life.s.setPosition(view.getCenter().x + 190 + i * 36, view.getCenter().y - 280);
		}
		if (p.life == false) {
			text.setPosition(view.getCenter().x - 50, view.getCenter().y);
			window.draw(text);
		}

		window.display();
	}
	return 0;
}



void gameRunning() {//ф-ция перезагружает игру , если это необходимо
	if (startGame()) { gameRunning(); }////если startGame() == true, то вызываем занова ф-цию isGameRunning, которая в свою очередь опять вызывает startGame() 
}






int main()
{
	gameRunning();//запускаем процесс игры
	return 0;
}
