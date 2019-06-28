#include <windows.h>
#include "fsUtility.h"

CFSLogger uLog;

void CFSLogger::out(string textout)
	{
		struct tm *zeit;
		time_t sec;
		time(&sec);
		zeit=localtime(&sec);
		
		ofstream LogDatei;
		LogDatei.open("log\\Debug.log", ios::out | ios::app);

		if(LogDatei.is_open())
			{
				//LogDatei << zeit->tm_mday << zeit->tm_mon << zeit->tm_year+1900 << "(" << zeit->tm_hour << ":" << zeit->tm_min << ":" << zeit->tm_sec << ") -> " << textout << endl;
				LogDatei << zeit->tm_hour << ":" << zeit->tm_min << ":" << zeit->tm_sec << " -> " << textout << endl;
				LogDatei.close();
			}
	}


CEngine::CEngine()
{
	uLog.out("## Starting up..");
	
	if (!ReadIniSettings())
		{
			uLog.out("ERROR Reading INI File");
			uLog.out("using savemode 800x600 windowed");
			m_scrWidth  = 1024;
			m_scrHeight = 768;
			m_windowmode= true;
		}

	m_GameState = 0;

}
CEngine::~CEngine()
{
	uLog.out("## Shutting down");
}

bool	CEngine::ReadIniSettings()
{

	ifstream iniFile("cfg\\game.cfg");
	uLog.out("parsing config file...");

	if(iniFile){
	
		char buffer[80];
		while(iniFile.getline(buffer, 80)){
		
			string s = buffer;
			if(s.find("//") == -1)
				{
					if (s.find("ScreenMode")!=-1)
					{
						string i = s.substr(11,1);
										
						switch(atoi(i.c_str()))
						{
						case 1 :
							  { 
								m_scrWidth = 800;
								m_scrHeight = 600;
								uLog.out("Entering mode 1 (800x600)");
							  }; break;
						case 2 :
							  { 
								m_scrWidth = 1024;
								m_scrHeight = 768;
								uLog.out("Entering mode 2 (1024x768)");
							  }; break;
						case  3:
							  { 
								m_scrWidth = 1280;
								m_scrHeight = 1024;
								uLog.out("Entering mode 3 (1280x1024)");
							  }; break;
						}

					}

					if (s.find("WindowMode")!=-1)
					{
						if(s.find("TRUE")!=-1)
							m_windowmode = true;
					} else {
							m_windowmode = false;
					}
				}
		}
		if (m_windowmode == true){
			uLog.out("Entering WINDOW Mode"); 
		} else {
			uLog.out("Entering FULLSCREEN Mode");
		}
		
		uLog.out("Config file successfully read...");
		return true;	
	} else {
		uLog.out("ERROR: could not open game.cfg");
		return false;
	}
}

short	CEngine::GetGameState()
{
	return m_GameState;
}

bool	CEngine::Init()
{
	return true;
}


CSnake::CSnake()
{
	length=1;
	lives=3;
	speed=20.0f;
	score=0;
	x=500;
	y=350;
}

CSnake::~CSnake()
{
	// blabla
}

bool CSnake::move()
{
	
return true;
}