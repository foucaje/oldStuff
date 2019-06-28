#ifndef _fsUtility_h
#define _fsUtility_h

#include <ctime>
#include <fstream>
#include <string>
using namespace std;

// ############################# LOG FILE CLASS ################################
struct CFSLogger
{
		
	void out(string textout);

};

// ############################# Engine CLASS ##################################

struct CEngine
{
	CEngine();
	~CEngine();

	short m_scrWidth;
	short m_scrHeight;
	short m_GameState;
	bool  m_windowmode;

	bool	ReadIniSettings();
	short	GetGameState();
	bool	Init();
	bool	newDrop();
};

struct CSnake
{
	CSnake();
	~CSnake();

	int		length;
	short	lives;
	float	speed;
	int		score;
	string	player;
	int		x,y;

	bool move();
};

#endif