#ifndef _player_h
#define _player_h

#include <ctime>
#include <fstream>
#include <string>
#include "fsUtility.h"

using namespace std;


struct CPlayer
{
	CPlayer();
	~CPlayer();

	
	string		m_playerName;
    short		m_games;
	short		m_points;
	short		m_highscore;
	short		m_difficulty;
	CFSLogger	m_Logger;
	
	bool	LoadStats();
	bool	SaveStats();
};

#endif