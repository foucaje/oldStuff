#include "player.h"

CPlayer::CPlayer()
{
    m_games			= 0;
	m_points		= 0;
	m_highscore		= 0;
	m_difficulty	= 1;
}

CPlayer::~CPlayer()
{

}

bool CPlayer::LoadStats()
{
		ifstream PlayerDB;
		PlayerDB.open("PlayerDB.dat", ios::in | ios::binary);
		if(!PlayerDB) m_Logger.out("ERROR reading PlayerDB");
return true;


}

bool CPlayer::SaveStats()
{
		ofstream PlayerDB;
		PlayerDB.open("PlayerDB.dat", ios::out | ios::binary);
		if(!PlayerDB) m_Logger.out("ERROR writing PlayerDB");
return true;
}