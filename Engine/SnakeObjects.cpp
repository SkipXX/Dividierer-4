#include "SnakeObjects.h"



//GameObject
GameObject::GameObject(int x, int y)
	:m_pos(x, y)
{

}

GameObject::GameObject(pair<int, int> pos)
	:m_pos(pos)
{

}

void GameObject::setCoords(int x, int y)
{
	m_pos.first = x;
	m_pos.second = y;
}










//Snake
Snake::Snake(int x, int y)
	:GameObject(x, y)
{

}

void Snake::move(int dx, int dy)
{
	if (m_tail.size() > 0) {
		for (int ii = 0; ii < m_tail.size() - 1; ii++)
		{
			m_tail.at(ii) = m_tail.at(ii + 1);
		}

		m_tail.at(m_tail.size() - 1) = m_pos;
	}

	m_pos.first += dx;
	m_pos.second += dy;

}

void Snake::eat(int dx, int dy)
{
	m_tail.push_back({ m_pos.first, m_pos.second });

	m_pos.first += dx;
	m_pos.second += dy;
}

void Snake::set_snake_dir(eDirection dir_snake)
{
	snake_dir = dir_snake;
}

eDirection Snake::get_snake_dir()
{
	return snake_dir;
}
