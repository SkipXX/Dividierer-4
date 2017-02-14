#pragma once

#include <vector>
#include <utility>
#include <vector>

using namespace std;

class GameObject
{
	friend class Game;
protected:
	pair<int, int> m_pos;

public:
	GameObject(int x=0, int y=0);
	GameObject(pair<int, int> pos);

	virtual void setCoords(int x, int y);
};




class Snake : public GameObject
{
	friend class Game;
private:
	vector<pair<int, int>> m_tail;
public:

	
	Snake(int x=0, int y=0);

	void move(int dx, int dy);
	void eat(int dx, int dy);
	//bool isDead();
};
