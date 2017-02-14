/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	Music(L"OdeAnDenDividierer.wav",0.0f,41.317f)
{	
	///
	width = 22;
	height = 22;

	m_snake.m_pos = { width / 2, height / 2 };

	softResetGame();

	m_music = true;
	Music.Play(1.0f, 0.5f);
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{	
	
	//if F is pressed
	if (wnd.kbd.KeyIsPressed(0x46))
	{
		respawnFruit();
	}
	//power cheat
	if (wnd.kbd.KeyIsPressed(0x50))
	{
		power_UP();
		Sleep(500);
	}
	//music
	if (wnd.kbd.KeyIsPressed(0x4D))
	{	
		if (m_music)
		{
			Music.StopAll();
			m_music = false;
		}
		else
		{
			Music.Play(1.0f, 0.5f);
			m_music = true;
			
		}
	}
	//Esc to exit
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		exit(1337);
	}

	//W for UP
	if (m_dir != UP && (m_dir != DOWN || score==0) && wnd.kbd.KeyIsPressed(0x57))
	{
		m_dir = UP;
	}
	//S for DOWN
	else if (m_dir != DOWN && (m_dir != UP || score == 0) && wnd.kbd.KeyIsPressed(0x53))
	{
		m_dir = DOWN;
	}
	//D for RIGHT
	else if (m_dir != RIGHT && (m_dir != LEFT || score == 0) && wnd.kbd.KeyIsPressed(0x44))
	{
		m_dir = RIGHT;
	}
	//A for LEFT
	else if (m_dir != LEFT && (m_dir != RIGHT || score == 0) && wnd.kbd.KeyIsPressed(0x41))
	{	
		m_dir = LEFT;
	}

	moveSnake(m_dir);

	//TRAP AND POWERUP SPAWN
	/*
	if(score != 0 && score % 3 == 0)
	{ 
		m_traps.push_back(emptySpace());
	}
	*/

	if (score / 4 > m_powerups.size() + power)
	{
		m_powerups.push_back(emptySpace());
	}

	

	//Framerate handling xd
	Sleep(100);
}

void Game::ComposeFrame()
{
	
	//BOARD
	for (auto& ii : m_traps)
	{
		if(m_immunity == 0) drawSquare(ii.m_pos.first, ii.m_pos.second, 200, 30, 30);
		else if(m_immunity > m_immunity_time + 1) drawSquare(ii.m_pos.first, ii.m_pos.second, 30, 100, 100);
		else drawSquare(ii.m_pos.first, ii.m_pos.second, 200, 100, 100);
	}

	for (auto& ii : m_powerups)
	{
		drawSquare(ii.m_pos.first, ii.m_pos.second, 30, 30, 200);
	}

	drawSquare(m_food.m_pos.first, m_food.m_pos.second, 30, 200, 30);

	for (auto& ii : m_snake.m_tail)
	{
		drawSquare(ii.first, ii.second, 200, 200, 200);
	}
	 
	drawSquare(m_snake.m_pos.first, m_snake.m_pos.second, 30, 130, 200);
}	


///
bool Game::isTrap(int x, int y)
{
	for (auto& ii : m_traps)
	{
		if (ii.m_pos.first == x && ii.m_pos.second == y) return true;
	}
	return false;
}

bool Game::isUpgrade(int x, int y)
{
	for (auto& ii : m_powerups)
	{
		if (ii.m_pos.first == x && ii.m_pos.second == y) return true;
	}
	return false;
}

bool Game::isTail(int x, int y)
{
	for (auto& ii : m_snake.m_tail)
	{
		if (ii.first == x && ii.second == y) return true;
	}
	return false;
}

bool Game::isFood(int x, int y)
{
	if (m_food.m_pos.first == x && m_food.m_pos.second == y)
	{
		return true;
	}
	else return false;
}

pair<int, int> Game::emptySpace()
{
	int tempx, tempy;
	do
	{
		tempx = rand() % width;
		tempy = rand() % height;

	} while (isTrap(tempx, tempy) || isTail(tempx, tempy) || (tempx == m_food.m_pos.first && tempy == m_food.m_pos.second) || isUpgrade(tempx, tempy) || (tempx == m_snake.m_pos.first && tempy == m_snake.m_pos.second));

	return{ tempx, tempy };
}

void Game::respawnFruit() {
	m_food.m_pos = emptySpace();
}

void Game::drawSquare(int x, int y, int r, int g, int b)
{
	assert(x <= width && y <= height && "Error: tried to draw square outside of window!! [Game::drawSquare(...)]");
	int temp_y{gfx.ScreenHeight/height};
	int temp_x{gfx.ScreenWidth/width};
	gfx.drawRect(x*temp_x, y*temp_y, temp_x, temp_y, r, g, b);
}

int Game::moveSnake(eDirection dir)
{
	int dx{ 0 }, dy{ 0 };

	int snake_x = m_snake.m_pos.first;
	int snake_y = m_snake.m_pos.second;

	switch (dir)
	{
	case STOP:
		break;
	case LEFT:
		dx = -1;
		break;
	case RIGHT:
		dx = 1;
		break;
	case UP:
		dy = -1;
		break;
	case DOWN:
		dy = 1;
		break;
	default:
		break;
	}
	
	assert((abs(dx) + abs(dy) == 1 || abs(dx) + abs(dy) == 0 )&& "Error: MOVE SOMEHOW IS MORE THAN 1?! [Game::moveSnake()]");

	//ISFOOD
	if (isFood(snake_x + dx, snake_y + dy))
		{
			m_snake.eat(dx, dy);
			respawnFruit();
			score++;

			m_traps.push_back(emptySpace());
			if(power > 4 || score > 18) m_traps.push_back(emptySpace());

			//m_immunity += 3;
		}

	//immunity trap eating :]
	else if (isTrap(snake_x + dx, snake_y + dy) && m_immunity > 0)
	{
		vector<GameObject> tempv;
		for (auto& ii : m_traps)
		{
			if (ii.m_pos.first != snake_x + dx || ii.m_pos.second != snake_y + dy)
			{
				tempv.push_back({ ii.m_pos.first, ii.m_pos.second });
			}
		}
		m_traps = tempv;

		m_snake.move(dx, dy);
	}
		//RIP RESET
		else if (isTrap(snake_x + dx, snake_y + dy) || isTail(snake_x + dx, snake_y + dy))
		{
			softResetGame();
		}
		//UPGRADE
		else if (isUpgrade(snake_x + dx, snake_y + dy))
		{
			power_UP();

			vector<GameObject> tempv;
			for (auto& ii : m_powerups)
			{
				if (ii.m_pos.first != snake_x + dx || ii.m_pos.second != snake_y + dy)
				{
					tempv.push_back({ ii.m_pos.first, ii.m_pos.second });
				}
			}
			m_powerups = tempv;
			
			m_snake.move(dx, dy);
		}
		//LOOPING
		else if (snake_x + dx < 0)
		{
			m_snake.move(width-1, 0);
		}
		else if (snake_x + dx > width-1)
		{
			m_snake.move(-(width-1),0);
		}
		else if (snake_y + dy < 0)
		{
			m_snake.move(0,height-1);
		}
		else if (snake_y + dy > height -1)
		{
			m_snake.move(0, -(height-1));
		}
		//NORMAL MOVE
		else m_snake.move(dx, dy);
	
	//reduce immunity
		if (m_immunity > 0)
		{
			m_immunity--;
		}


	return 0;
}

void Game::softResetGame()
{
	power = 0;
	score = 0;
	m_immunity = 0;

	m_traps.clear();
	m_powerups.clear();
	m_snake.m_tail.clear();

	
	//Walls are beeing created with Traps
	for (int ii = 0; ii < width; ii++)
	{
		for (int jj = 0; jj < height; jj++)
		{
			if (ii == 0 || ii == width - 1)
			{
				m_traps.push_back({ ii, jj });
			}
			else
			{
				if (jj == 0 || jj == height - 1)
				{
					m_traps.push_back({ ii,jj });
				}
			}
		}
	}
	
	respawnFruit();
	///
}

void Game::power_UP()
{	
	power++;

	int immunity_time = m_immunity_time*power;

	//remove walls
	if (power == 1)
	{
		vector<GameObject> tempv;
		for (auto& ii : m_traps)
		{
			if (ii.m_pos.first != 0 || ii.m_pos.second == 0 || ii.m_pos.second == height - 1)
			{
				tempv.push_back(ii);
			}
		}
		m_traps = tempv;
	}
	else if (power == 2)
	{
		vector<GameObject> tempv;
		for (auto& ii : m_traps)
		{
			if (ii.m_pos.first != width - 1 || ii.m_pos.second == 0 || ii.m_pos.second == height - 1)
			{
				tempv.push_back(ii);
			}
		}
		m_traps = tempv;

		m_immunity = immunity_time;
	}
	else if (power == 3)
	{
		vector<GameObject> tempv;
		for (auto& ii : m_traps)
		{
			if (ii.m_pos.second != 0)
			{
				tempv.push_back(ii);
			}
		}
		m_traps = tempv;

		m_immunity = immunity_time;
	}
	else if (power == 4)
	{
		vector<GameObject> tempv;
		for (auto& ii : m_traps)
		{
			if (ii.m_pos.second != height-1)
			{
				tempv.push_back(ii);
			}
		}
		m_traps = tempv;

		m_immunity = immunity_time;
	}

	//immunity
	else if (power > 4)
	{
		m_immunity = m_immunity_time * 5;
	}

	//compensate for immunity loss after moving
	m_immunity++;
}
///