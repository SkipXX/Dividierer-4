/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

#include "Sound.h"

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

#include "SnakeObjects.h"

enum eDirection {
	STOP,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */


	bool isTail(int x, int y);
	bool isFood(int x, int y);
	bool isTrap(int x, int y);
	bool isUpgrade(int x, int y);

	pair<int, int> emptySpace();
	pair<int, int> emptySpaceNotNearSnake();

	void respawnFruit();
	void drawSquare(int x, int y, int r, int g, int b);
	void drawCircle(int x, int y, Color color);
	void drawCircle(int x, int y, int r, int g, int b);
	int moveSnake(eDirection dir);
	void softResetGame();
	void power_UP();
	void spawnTrap(int n);
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;

	/********************************/
	/*  User Variables              */
	int width, height;
	int score;
	int power;
	eDirection m_dir;

	int m_immunity;
	static constexpr int m_immunity_time = 6;

	Snake m_snake;
	GameObject m_food;
	vector<GameObject> m_traps;
	vector<GameObject> m_powerups;

	bool m_music;
	Sound Music;

	static constexpr int frameDelay = 6;
	int frameCounter = 0;
	bool pause = false;
	/********************************/
};