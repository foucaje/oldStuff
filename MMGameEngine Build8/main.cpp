/********************************************************/
/*	Project:		nexusCreations 2d Game SDK			*/
/*	Author:			jérôme foucauld						*/
/*	Date:			18.Feb.2005							*/
/*	Platform:		win32 / DirectX						*/
/*	last edited:	19.Feb.2005							*/
/********************************************************/

// Headerdateien
#include "DirectXGraphics.h"
#include "sprite.h"
#include "RessourceManager.h"
#include "InputManager.h"
#include "DXAudioClass.h"
#include "GameManager.h"
#include <ctime>

//#include "CollisionArea.h"

#define VERSION 0.8
char dateString[] = "20.Feb.2005";

short FrameRate = 30;

// Anwendungsfenster erzeugen
HWND CreateMainWindow(HINSTANCE hInstance);

// Callback Funktion zur Nachrichtenbehandlung
LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// Das Fensterhandle
HWND hWnd = 0;
HINSTANCE hInst = 0;

CDirectXGraphics	Direct3D;				// Direct3D
CEngine				Game;					// todo: entfernen
CFSLogger			mLog;					// todo: entfernen
CResourceManager	ResourceManager;		// todo: ausarbeiten
CInputManager		InputManager;			// DirectInput
DirectXAudio		DXAudio;				// DXAudio
CGameManager		GameManager;			// todo: verbessern
CSprite				Intro;					// ok
CSprite				Main;					// ok
CSprite				GameMain;			    // ok
CSprite				B1,B2,B3,B4,B5,B6,B7,B8;// ok
CSprite				hook,arrow,cross;		// ok
CSprite				won,lost,opt;			// ok


char StatusBuffer[256];
char TextBuffer[256];

char InputBuffer[128];
bool userRegged = false;
bool userSelect = false;
bool userDiff = false;
short InputIndex = 0;

BOOL InitGame(void);
short currentTry(void);
int checkColor();
void printCurrentTry();
void printAllTrys();
void printAllFlags();
void printStats();
bool LClick = false;
bool RClick = false;


LONGLONG Frequency;
LONGLONG Offset;
LONGLONG NextFrame;
LONGLONG CurCount;
int MouseX, MouseY;
short Code[5]		= { 1,1,1,1,1 };		// voreingestellt
short Try[16][5]	= { 0 };
short Report[16][5]	= { 0 };
short offset = 0;
short xy[5][2]		= { {491,630},{531,630},{571,630},{611,630},{651,630} };
short TryIndex = 1;
bool checkCode();
void ClearAllTrys();
void GenerateCode();
void getMenuClick();
void GetString(int key);


// Windows main-Funktion
int WINAPI WinMain(HINSTANCE hInstance,      // Handle der Programminstanz
                   HINSTANCE hPrevInstance,  // Handle der letzten Instanz
                   LPSTR lpCmdLine,          // Kommandozeile
                   int nCmdShow)             // Art wie das Fenster angezeigt werden soll
{
    // Fenster erzeugen und Handle speichern
    hWnd = CreateMainWindow(hInstance);
	hInst = hInstance;

    // Wenn der Rückgabewert 0 ist, ist ein Fehler aufgetreten
    if(0 == hWnd)
    {
		mLog.out("ERROR: Fenster konnte nicht erzeugt werden( hWnd returned 0 )");
		return -1;
    }
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);
	Offset = Frequency/FrameRate;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurCount);

	InitGame();
	GenerateCode();		
	// Struktur, in der Informationen zur Nachricht gespeichert werden
	MSG msg = {0};
	NextFrame=CurCount + Offset;

    // Diese Schleife läuft bis die Nachricht WM_QUIT empfangen wird
    while(msg.message != WM_QUIT)
	{
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				// Nachricht an die Callbackfunktion senden
				TranslateMessage(&msg);
		        DispatchMessage(&msg);
			} else {
				
				QueryPerformanceCounter((LARGE_INTEGER*)&CurCount);

				
				ResourceManager.Update();
				
				// Intro
				if (Game.GetGameState() == 0) 
				{
					Direct3D.BeginScene();
					int Key = InputManager.GetInput();
					Intro.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
		
					if (Key==99 || GetAsyncKeyState(VK_LBUTTON))
					{
						Game.m_GameState = 3;
						LClick = false;
						GameManager.Reset();
					}
					
					Direct3D.EndScene();
				}	
				// HauptMenü
				if (Game.GetGameState() == 3)			
				{
					Direct3D.BeginScene();
					int Key = InputManager.GetInput();
					Main.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					
					
				if (userRegged==false)
					{
						opt.Draw();
						sprintf(TextBuffer,"What's your Name? :");
						
						Direct3D.DrawText(TextBuffer,290,200,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
						//GetString(Key);
						Direct3D.DrawText(InputBuffer,650,200,D3DCOLOR_XRGB(0xEA, 0xFF, 0));
						
						
							if (Key==230)
							{
								sprintf(GameManager.PlayerName,"%s",InputBuffer);
								userRegged=true;
							}
						
						
						if (Key == 16) 
						{
							GameManager.Difficulty=1;
						}
						
						if (Key == 32) 
						{
							GameManager.Difficulty=2;
						}
						sprintf(TextBuffer,"Difficulty Settings: (toggle F1/F2)");
						Direct3D.DrawText(TextBuffer,290,220,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
						char dif[10];
						if (GameManager.Difficulty==1){ strcpy(dif,"beginner"); } else { strcpy(dif,"pro"); }
						Direct3D.DrawText(dif,650,220,D3DCOLOR_XRGB(0xEA, 0xFF, 0));
						sprintf(TextBuffer,"Select your difficulty setting.");
						Direct3D.DrawText(TextBuffer,290,300,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
						sprintf(TextBuffer,"Then enter your name and press ENTER to start.");
						Direct3D.DrawText(TextBuffer,290,320,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					}
						
						
					
					
					if (userRegged==true) getMenuClick();
					
					Direct3D.EndScene();
				}		
				// Game Läuft im SinglePlayer
				if (Game.GetGameState() == 1)
				{
					Direct3D.BeginScene();
					GameMain.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

					int Key = InputManager.GetInput();
					/*
					if (Key == 32) GameManager.Difficulty = 2;
					if (Key == 16) GameManager.Difficulty = 1;
					*/
					
					if (RClick==true)
					{
						Try[0][currentTry()-1] = 0;
						RClick=false;
					}
					if (LClick==true)
					{
						
					if ((MouseX > 336 && MouseX < 686) && (MouseY > 604 && MouseY < 654))	// Eingabezurücknehmen
							{
								Try[0][currentTry()-1] = 0;
							}

						switch(checkColor())
						{
						case 1:
							Try[0][currentTry()]=1;
							break;
						case 2:
							Try[0][currentTry()]=2;
							break;
						case 3:
							Try[0][currentTry()]=3;
							break;
						case 4:
							Try[0][currentTry()]=4;
							break;
						case 5:
							Try[0][currentTry()]=5;
							break;
						case 6:
							Try[0][currentTry()]=6;
							break;
						case 7:
							Try[0][currentTry()]=7;
							break;
						case 8:
							Try[0][currentTry()]=8;
							break;
						}

						LClick = false;					
					}
					
					printCurrentTry();
					printAllTrys();
					printAllFlags();
					if (currentTry()>4)
					{
						if (checkCode())
						{
							Game.m_GameState=5;
						}
						if (TryIndex==16)
						{
							GameManager.Games++;
							GameManager.HighScore=(float)GameManager.m_Points/(float)GameManager.Games;
							Game.m_GameState=6;
						}
					}
					printStats();
					Direct3D.EndScene();
				}
				// Code geknackt!
				if (Game.GetGameState() == 5)	// Code geknackt!
				{
					Direct3D.BeginScene();
					GameMain.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

					won.SetPosition(512.0f,364.0f);
					won.Draw();

					int Key = InputManager.GetInput();

					if (Key == 99 || LClick==true)
					{
						LClick=false;
						ClearAllTrys();
						GenerateCode();
						TryIndex=1;
						Game.m_GameState=1;
					}
					
					printStats();
					
					Direct3D.EndScene();

				}
				// RundeVerloren
				if (Game.GetGameState() == 6)
				{
					Direct3D.BeginScene();
					GameMain.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
	
					sprintf(TextBuffer,"You got busted, loser!!!\n\nSpace to continue");
					Direct3D.DrawText(TextBuffer,450,360,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));


					int Key = InputManager.GetInput();

					if (Key == 99 || LClick==true)
					{
						LClick=false;
						ClearAllTrys();
						GenerateCode();
						TryIndex=1;
						Game.m_GameState=1;
					}
					
					printStats();
					
					Direct3D.EndScene();
				}
				// QuitToDesktop
				if (Game.GetGameState() == 99)			
				{
					Direct3D.BeginScene();
					int Key = InputManager.GetInput();
					Main.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					opt.Draw();
					sprintf(TextBuffer,"Thank you for playing Mastermind =)\nclick to return to desktop");
					Direct3D.DrawText(TextBuffer,395,300,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"Your highscore today: %d in %d games", GameManager.m_Points, GameManager.Games);
					Direct3D.DrawText(TextBuffer,395,230,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
										
					
					if (Key==99 || LClick==true)
						{	
							DestroyWindow(hWnd);
						}

					Direct3D.EndScene();
				}		
				// HowToPlay
				if (Game.GetGameState() == 10)			
				{
					Direct3D.BeginScene();
					int Key = InputManager.GetInput();
					Main.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					opt.Draw();
					sprintf(TextBuffer,"Welcome to nexusCreations MasterMind\n____________________________________");
					Direct3D.DrawText(TextBuffer,365,140,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"Your Goal is to crack the ColorCode, containing 5 of the 8 Colors.");
					Direct3D.DrawText(TextBuffer,290,200,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"You have a total of 8 Trys to win the Round.");
					Direct3D.DrawText(TextBuffer,290,220,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"Simply click the ColorSelector on the right to select a color, or use");
					Direct3D.DrawText(TextBuffer,290,240,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"rightclick to undo a step.");
					Direct3D.DrawText(TextBuffer,290,260,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"After selecting the fifth color your cracking attemp is being analyzed.");
					Direct3D.DrawText(TextBuffer,290,290,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"Your Combination is displayd along with some helpers depending on difficulty setting:");
					Direct3D.DrawText(TextBuffer,290,310,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"- the X shows this color is not in the code");
					Direct3D.DrawText(TextBuffer,320,330,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"- the <-> shows this color is valid but not on its right position");
					Direct3D.DrawText(TextBuffer,320,350,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"- the hook shows this color is on its correct position");
					Direct3D.DrawText(TextBuffer,320,370,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					if (LClick)
					{Game.m_GameState=3;}
					Direct3D.EndScene();
				}
				// Credits
				if (Game.GetGameState() == 11)			
				{
					Direct3D.BeginScene();
					int Key = InputManager.GetInput();
					
					Main.Draw();
					opt.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					sprintf(TextBuffer,"C R E D I T S");
					Direct3D.DrawText(TextBuffer,475,140,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"programming by:");
					Direct3D.DrawText(TextBuffer,290,200,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"jérôme foucauld");
					Direct3D.DrawText(TextBuffer,620,200,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					sprintf(TextBuffer,"music by:");
					Direct3D.DrawText(TextBuffer,290,220,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"jérôme foucauld");
					Direct3D.DrawText(TextBuffer,620,220,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					sprintf(TextBuffer,"artwork by:");
					Direct3D.DrawText(TextBuffer,290,240,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"jérôme foucauld");
					Direct3D.DrawText(TextBuffer,620,240,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					sprintf(TextBuffer,"project design by:");
					Direct3D.DrawText(TextBuffer,290,260,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"jérôme foucauld");
					Direct3D.DrawText(TextBuffer,620,260,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					
					sprintf(TextBuffer,"version:");
					Direct3D.DrawText(TextBuffer,290,300,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"%2.2f beta dated (%s)", VERSION, dateString);
					Direct3D.DrawText(TextBuffer,528,300,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

					sprintf(TextBuffer,"i would like to thank my gf caro for supporting and inspirating me\n\nas well as "
						"everyone else who likes this little game :)\n\n\nif you encounter any problems please drop me a note under:\n"
						"jerome@nexuscreations.net\n\n\n(c)'2005 nexusCreations");
					Direct3D.DrawText(TextBuffer,305,400,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					
					if (LClick)
					{Game.m_GameState=3;}
					Direct3D.EndScene();
				}
				// HighScore
				if (Game.GetGameState() == 12)			
				{
					Direct3D.BeginScene();
					int Key = InputManager.GetInput();
					
					Main.Draw();
					opt.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					sprintf(TextBuffer,"H I G H S C O R E");
					Direct3D.DrawText(TextBuffer,458,140,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"not yet implemented");
					Direct3D.DrawText(TextBuffer,290,200,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					if (LClick)
					{Game.m_GameState=3;}
					Direct3D.EndScene();
				}
				// Optionen
				if (Game.GetGameState() == 20)			
				{
					Direct3D.BeginScene();
					int Key = InputManager.GetInput();

					/*
					if (Key == 16) 
					{
						GameManager.Difficulty=1;
					}
					if (Key == 32) 
					{
						GameManager.Difficulty=2;
					}
					*/
					if (Key == 64) 
					{
							GameManager.Sound=true;
					}

					if (Key == 128) 
					{
							GameManager.Sound=false;		
					}

					
					Main.Draw();
					opt.Draw();
					sprintf(TextBuffer,"Version: %2.1f beta",VERSION);
					Direct3D.DrawText(TextBuffer,5,750,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					sprintf(TextBuffer,"O P T I O N S");
					Direct3D.DrawText(TextBuffer,458,140,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					
					/*
					sprintf(TextBuffer,"Difficulty Settings: (toggle F1/F2)");
					Direct3D.DrawText(TextBuffer,290,200,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					char dif[10];
					if (GameManager.Difficulty==1){ strcpy(dif,"beginner"); } else { strcpy(dif,"pro"); }
					Direct3D.DrawText(dif,650,200,D3DCOLOR_XRGB(0xEA, 0xFF, 0));
					*/					
					
					sprintf(TextBuffer,"Music / Sound: (toggle F5/F6)");
					Direct3D.DrawText(TextBuffer,290,220,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					char dif[10];
					if (GameManager.Sound==true){ strcpy(dif,"on"); } else { strcpy(dif,"off"); }
					Direct3D.DrawText(dif,650,220,D3DCOLOR_XRGB(0xEA, 0xFF, 0));
					
					
					if (LClick)
					{Game.m_GameState=3;}
					Direct3D.EndScene();
				}

				LClick=false;

			}
	
	}

	
    // Rückgabewert an Windows
    return 0;
}

// Fenster erzeugen
HWND CreateMainWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wndClass =
    {
        sizeof(WNDCLASSEX),                                 // Größe angeben
        CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW,    // Standardstile
        MessageHandler,                                     // Callback-Funktion
        0,                                                  // Zusätzliche Angaben
        0,                                                  // nicht benötigt
        hInstance,                                          // Anwendungsinstanz
        LoadIcon(NULL, IDI_WINLOGO),                        // Windows-Logo
        LoadCursor(NULL, IDC_ARROW),                        // Normaler Cursor
        (HBRUSH)GetStockObject(WHITE_BRUSH),                // Weißer Pinsel
        NULL,                                               // kein Menü
        "WindowClass",                                      // Der Name der Klasse
        LoadIcon(NULL, IDI_WINLOGO)                         // Windows Logo
    };

    // Klasse registrieren
    RegisterClassEx(&wndClass);

    return CreateWindowEx(NULL,										// Keine erweiterten Stile nutzen
						  "WindowClass",						    // Klassenname
                          "mastermind (c)'2005 nexusCreations",		// Fenstertitel
                          WS_POPUP |								// Fenster
                          WS_VISIBLE,								// Eigenschaften
                          0, 0,										// Anfangsposition
						  Game.m_scrWidth, Game.m_scrHeight,		// und Größe des Fenster
                          NULL,										// Handle des Elternfensters
                          NULL,										// Handle des Menüs
                          hInstance,								// Anwendungsinstanz
                          NULL);									// wird nicht benötigt
}


// Diese Funktion wird von Windows aufgrufen, wenn eine Nachricht vorliegt
LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    // testen, um welche Nachticht es sich handelt
    switch(msg)
    {
        // Programm beenden, wenn das Fenster
        // geschlossen wird
        case WM_DESTROY:
				{
				PostQuitMessage(0);
				return 0;
				}
             break;

        case WM_KEYDOWN:
               switch(wParam)
               {
                 //Wenn ESC gedrückt
                 case VK_ESCAPE:
					 {
							 Game.m_GameState=3;					 
					 }
                  break;
               }
            
		case WM_MOUSEMOVE:
			{
				MouseX = LOWORD(lParam);
				MouseY = HIWORD(lParam);
			} break;
		case WM_LBUTTONDOWN:
			{
				LClick = true;
			} break;
		case WM_RBUTTONDOWN:
			{
				RClick = true;
			} break;
		case WM_CHAR:
			{
				if (userRegged==false)
				{
					GetString(InputManager.GetInput());
				}	
			}


	break;
    }

    // Standardnachrichtenverarbeitung von Windows
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Spiel initialisieren
BOOL InitGame(void)
{   
    // Zufallsgenerator initialisieren
    srand(timeGetTime());

    // Direct3D initialisieren
    if (!Direct3D.Init(hWnd, Game.m_scrWidth, Game.m_scrHeight, Game.m_windowmode)){
	
		mLog.out("ERROR DirectGraphics Init fehlgeschlagen!");
	}

//	if(!Game.m_windowmode)
  //  {
       ShowCursor(TRUE);
   // }

    // Farbe die beim Löschen verwendet werden soll
    Direct3D.SetClearColor(D3DCOLOR_XRGB(0xFF,0xFF,0xFF));


    // ResourceManager initialisieren
    if(!ResourceManager.Init(Direct3D.GetDevice()))
    {
        mLog.out("ERROR ResourceManager Init fehlgeschlagen!");
		return FALSE;
    }

    // InputManager initialisieren
    if(!InputManager.Init(hWnd,hInst))
    {
        mLog.out("ERROR DInputManager Init fehlgeschlagen!");
		return FALSE;
    }
	
    // DirectX Audio initialisieren
    DXAudio.Init();

	// Hintergrundmusik laden und abspielen
    // DXAudio.AddBackgroundMusic("musik.mid");
    // DXAudio.PlayBackgroundMusic();

    // Soundeffekte laden
    // DXAudio.AddSound("gameover.wav");
    
    // GameManager initialisieren
	GameManager.Init(Direct3D.GetDevice(), &ResourceManager, &DXAudio);
	
	// Texturen laden
	Intro.Create(Direct3D.GetDevice(),"res\\ncintro.png");
	Main.Create(Direct3D.GetDevice(),"res\\ncmenu.png");
	GameMain.Create(Direct3D.GetDevice(),"res\\gamemain.png");
	B1.Create(Direct3D.GetDevice(),"res\\B1.png");
	B2.Create(Direct3D.GetDevice(),"res\\B2.png");
	B3.Create(Direct3D.GetDevice(),"res\\B3.png");
	B4.Create(Direct3D.GetDevice(),"res\\B4.png");
	B5.Create(Direct3D.GetDevice(),"res\\B5.png");
	B6.Create(Direct3D.GetDevice(),"res\\B6.png");
	B7.Create(Direct3D.GetDevice(),"res\\B7.png");
	B8.Create(Direct3D.GetDevice(),"res\\B8.png");
	hook.Create(Direct3D.GetDevice(),"res\\hook.png");
	arrow.Create(Direct3D.GetDevice(),"res\\arrow.png");
	cross.Create(Direct3D.GetDevice(),"res\\cross.png");
	won.Create(Direct3D.GetDevice(),"res\\wonsmall.png");
	opt.Create(Direct3D.GetDevice(),"res\\opt.png");
	opt.SetPosition(512.0f,384.0f);

	  	
	mLog.out("InitGame() successfull");
    return TRUE;
}



short currentTry(void)
{
	//sprintf(TextBuffer,"sTry[0]-> %d %d %d %d %d", Try[0][0],Try[0][1],Try[0][2],Try[0][3],Try[0][4]);
	//Direct3D.DrawText(TextBuffer,42,300,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));	
	
	for(int i=0;i<5;i++)
	{
	
		if (Try[0][i] == 0)
			{
				return i;
			}
			
	}

}

int checkColor()
{
	if(MouseX > 863 && MouseX < 922) // linke Kugelhälfte
	{
		if (MouseY > 247 && MouseY < 326)
			return 2;
		if (MouseY > 326 && MouseY < 407)
			return 4;
		if (MouseY > 407 && MouseY < 487)
			return 6;
		if (MouseY > 487 && MouseY < 568)
			return 8;
	}

	if(MouseX > 922 && MouseX < 984) // rechte Kugelhälfte
	{
		if (MouseY > 207 && MouseY < 287)
			return 1;
		if (MouseY > 278 && MouseY < 367)
			return 3;
		if (MouseY > 367 && MouseY < 447)
			return 5;
		if (MouseY > 447 && MouseY < 528)
			return 7;
	}

return 0;
}

void printCurrentTry()
{
	for (int i=0;i<5;i++)
	{
		if (Try[0][i] != 0)
		{
			int farbe = Try[0][i];
            switch(farbe)
				{
				case 1:
					{
						B1.SetPosition(xy[i][0],xy[i][1]);
						B1.Draw();
					} 
						break;
				case 2:
					{
						B2.SetPosition(xy[i][0],xy[i][1]);
						B2.Draw();
					} 
						break;
				case 3:
					{
						B3.SetPosition(xy[i][0],xy[i][1]);
						B3.Draw();
					} 
						break;
				case 4:
					{
						B4.SetPosition(xy[i][0],xy[i][1]);
						B4.Draw();
					} 
						break;
				case 5:
					{
						B5.SetPosition(xy[i][0],xy[i][1]);
						B5.Draw();
					} 
						break;
				case 6:
					{
						B6.SetPosition(xy[i][0],xy[i][1]);
						B6.Draw();
					} 
						break;
				case 7:
					{
						B7.SetPosition(xy[i][0],xy[i][1]);
						B7.Draw();
					} 
						break;
				case 8:
					{
						B8.SetPosition(xy[i][0],xy[i][1]);
						B8.Draw();
					} 
						break;

				}
			
		}
	}
}

bool checkCode()
{
	//char buff[50];
	//sprintf(buff, "checking code with try %d and index %d", currentTry(), TryIndex);
	//mLog.out(buff);
	Try[TryIndex][0]=Try[0][0];
	Try[TryIndex][1]=Try[0][1];
	Try[TryIndex][2]=Try[0][2];
	Try[TryIndex][3]=Try[0][3];
	Try[TryIndex][4]=Try[0][4];
	
	Try[0][0] = 0;
	Try[0][1] = 0;
	Try[0][2] = 0;
	Try[0][3] = 0;
	Try[0][4] = 0;
	
    
	for (int c=0;c<5;c++)
	{
		for(int d=0;d<5;d++)
		{

//			if (Code[c] == Try[TryIndex][c])
//			{ 
//				Report[TryIndex][c]=1;
//			} 
			
			if (Code[c]==Try[TryIndex][d]) {
		      Report[TryIndex][d]=2;		
			}
			
			if (Code[c] == Try[TryIndex][c])
			{ 
				Report[TryIndex][c]=1;
			} 
		}
	}

	if (Try[TryIndex][0]==Code[0] &&
		Try[TryIndex][1]==Code[1] &&
		Try[TryIndex][2]==Code[2] &&
		Try[TryIndex][3]==Code[3] &&
		Try[TryIndex][4]==Code[4])
		{
			if (GameManager.Best>TryIndex || GameManager.Best==0) GameManager.Best=TryIndex;
			GameManager.Games++;
			if (GameManager.Difficulty == 1) GameManager.m_Points+=160-(TryIndex*10);
			if (GameManager.Difficulty == 2) GameManager.m_Points+=160-(TryIndex*3);
			GameManager.HighScore=(float)GameManager.m_Points/(float)GameManager.Games;
			return true;
		}

	TryIndex++;
	return false;
}

void printAllTrys()
{
	for (int h=1;h<TryIndex;h++)
	{
		for (int i=0;i<5;i++)
		{
				int farbe = Try[h][i];
			    switch(farbe)
					{
					case 1:
						{
							B1.SetPosition((float)xy[i][0],((float)xy[i][1]-26)-(30*h));
							B1.Draw();
						} 
							break;
					case 2:
						{
							B2.SetPosition((float)xy[i][0],((float)xy[i][1]-26)-(30*h));
							B2.Draw();
						} 
							break;
					case 3:
						{
							B3.SetPosition((float)xy[i][0],((float)xy[i][1]-26)-(30*h));
							B3.Draw();
						} 
							break;
					case 4:
						{
							B4.SetPosition((float)xy[i][0],((float)xy[i][1]-26)-(30*h));
							B4.Draw();
						} 
							break;
					case 5:
						{
							B5.SetPosition((float)xy[i][0],((float)xy[i][1]-26)-(30*h));
							B5.Draw();
						} 
							break;
					case 6:
						{
							B6.SetPosition((float)xy[i][0],((float)xy[i][1]-26)-(30*h));
							B6.Draw();
						} 
							break;
					case 7:
						{
							B7.SetPosition((float)xy[i][0],((float)xy[i][1]-26)-(30*h));
							B7.Draw();
						} 
							break;
					case 8:
						{
							B8.SetPosition((float)xy[i][0],((float)xy[i][1]-26)-(30*h));
							B8.Draw();
						} 
							break;

					}
						
		}
	}
}

void printAllFlags()
{

	for (int h=1;h<TryIndex;h++)
	{
		int x=0,y=0,z=0;

		if (TryIndex==1) break;
		for (int i=0;i<5;i++)
		{
			if (GameManager.Difficulty==1)
			{
				int farbe = Report[h][i];
			    switch(farbe)
					{
					case 0:
						{
							cross.SetPosition((float)xy[i][0]+10,(float)xy[i][1]-26-(30*h)+10);
							cross.Draw();
						} 
							break;
					case 1:
						{
							hook.SetPosition((float)xy[i][0]+10,(float)xy[i][1]-26-(30*h)+10);
							hook.Draw();
						} 
							break;
					case 2:
						{
							arrow.SetPosition((float)xy[i][0]+10,(float)xy[i][1]-26-(30*h)+10);
							arrow.Draw();
						} 
							break;
					}
			}
			if (Report[h][i] == 0) x++;
			if (Report[h][i] == 1) y++;
			if (Report[h][i] == 2) z++;
		}
		if (GameManager.Difficulty==2)
		{
			/*cross.SetPosition((float)270,(float)630-(60*h));
			cross.Draw();
			sprintf(TextBuffer,"%d", x);
			Direct3D.DrawText(TextBuffer,280,630-(60*h)-5,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));*/

			hook.SetPosition((float)270,(float)604-(30*h));
			hook.Draw();
			sprintf(TextBuffer,"%d", y);
			Direct3D.DrawText(TextBuffer,280,604-(30*h)-8,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

			arrow.SetPosition((float)305,(float)604-(30*h));
			arrow.Draw();
			sprintf(TextBuffer,"%d", z);
			Direct3D.DrawText(TextBuffer,320,604-(30*h)-8,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

		}
	}

}

void ClearAllTrys()
{
	for (int h=0;h<16;h++)
	{
		for (int i=0;i<5;i++)
		{
			Try[h][i]=0;
			Report[h][i]=0;
		}
	}
}
void GenerateCode()
{
	srand(timeGetTime());
	
	Code[0] = rand()%8+1;
	Code[1] = rand()%8+1;
	Code[2] = rand()%8+1;
	Code[3] = rand()%8+1;
	Code[4] = rand()%8+1;
	int x=0;
	bool invalid=true;
	while(x<5)
	{
		if (Code[1] == Code[0]) Code[1]=rand()%8+1;
		if (Code[2] == Code[0] || Code[2] == Code[1]) Code[2]=rand()%8+1;
		if (Code[3] == Code[0] || Code[3] == Code[1] || Code[3] == Code[2]) Code[3]=rand()%8+1;
		if (Code[4] == Code[0] || Code[4] == Code[1] || Code[4] == Code[2] || Code[4] == Code[3]) Code[4]=rand()%8+1;
		x++;
	}
	

	
}

void getMenuClick()
{
	if(MouseX > 363 && MouseX < 663) // MenuZentrun
	{
		if (MouseY > 234 && MouseY < 264)
		{
			if (LClick){ Game.m_GameState=1; LClick=false; }
			/*sprintf(TextBuffer,"Mouse Over -> 1");
			Direct3D.DrawText(TextBuffer,80,230,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));*/
		}

		if (MouseY > 330 && MouseY < 360)
		{
			if (LClick){ Game.m_GameState=12; LClick=false; }
	
		}
		if (MouseY > 370 && MouseY < 400)
		{
			if (LClick){ Game.m_GameState=10; LClick=false; }
	
		}
		if (MouseY > 410 && MouseY < 430)
		{
			if (LClick){ Game.m_GameState=11; LClick=false; }
		}
		
		if (MouseY > 440 && MouseY < 470)
		{
			if (LClick){ Game.m_GameState=20; LClick=false; }
		}

		if (MouseY > 506 && MouseY < 536)
		{
			if (LClick){ Game.m_GameState=99; LClick=false; }
		}
	}
}

void GetString(int key)
{
	
	switch(key)
	{
	case 200:
		sprintf(InputBuffer,"%sq",InputBuffer);
		break;
		case 201:
		sprintf(InputBuffer,"%sw",InputBuffer);
		break;
		case 202:
		
		sprintf(InputBuffer,"%se",InputBuffer);
		break;
		case 203:
		
		sprintf(InputBuffer,"%sr",InputBuffer);
		break;
		case 204:
		
		sprintf(InputBuffer,"%st",InputBuffer);
		break;
		case 205:
		sprintf(InputBuffer,"%sz",InputBuffer);
		break;
		case 206:
		sprintf(InputBuffer,"%su",InputBuffer);
		break;
		case 207:
		sprintf(InputBuffer,"%si",InputBuffer);
		break;
		case 208:
		sprintf(InputBuffer,"%so",InputBuffer);
		break;
		case 209:
		sprintf(InputBuffer,"%sp",InputBuffer);
		break;

		case 210:
			sprintf(InputBuffer,"%sa",InputBuffer);
		
		break;
		case 211:
			sprintf(InputBuffer,"%ss",InputBuffer);
		
		break;
		case 212:
			sprintf(InputBuffer,"%sd",InputBuffer);
		
		break;
		case 213:
			sprintf(InputBuffer,"%sf",InputBuffer);
		
		break;
		case 214:
			sprintf(InputBuffer,"%sg",InputBuffer);
		
		break;
		case 215:
			sprintf(InputBuffer,"%sh",InputBuffer);
		
		break;
		case 216:
			sprintf(InputBuffer,"%sj",InputBuffer);
		
		break;
		case 217:
			sprintf(InputBuffer,"%sk",InputBuffer);
		
		break;
		case 218:
			sprintf(InputBuffer,"%sl",InputBuffer);
		
		break;
		
		case 219:
			sprintf(InputBuffer,"%sy",InputBuffer);
		
		break;
		case 220:
			sprintf(InputBuffer,"%sx",InputBuffer);
		break;
		case 221:
			sprintf(InputBuffer,"%sc",InputBuffer);
		break;
		case 222:
			sprintf(InputBuffer,"%sv",InputBuffer);
		
		break;
		case 223:
			sprintf(InputBuffer,"%sb",InputBuffer);
		
		break;
		case 224:
			sprintf(InputBuffer,"%sn",InputBuffer);
		
		break;
		case 225:
		sprintf(InputBuffer,"%sm",InputBuffer);
		
		break;
		case 99:
			sprintf(InputBuffer,"%s ",InputBuffer);
		
		break;
		case 231:
			{
				InputBuffer[strlen(InputBuffer)-1] = '\0';
			}

		break;
	}

	//if(key!=231)
	//{
//		InputIndex++;
	//}

}
void printStats()
{
					sprintf(TextBuffer,"Player: ");
					Direct3D.DrawText(TextBuffer,42,170,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"%s", GameManager.PlayerName);
					Direct3D.DrawText(TextBuffer,120,170,D3DCOLOR_XRGB(0xEA, 0xFF, 0));

					sprintf(TextBuffer,"Difficulty: ");
					Direct3D.DrawText(TextBuffer,42,190,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					char dif[10];
					if (GameManager.Difficulty==1){ strcpy(dif,"beginner"); } else { strcpy(dif,"pro"); }
					sprintf(TextBuffer,"%s", dif);
					Direct3D.DrawText(dif,120,190,D3DCOLOR_XRGB(0xEA, 0xFF, 0));

					sprintf(TextBuffer,"Games: ");
					Direct3D.DrawText(TextBuffer,42,210,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"%d",GameManager.Games);
					Direct3D.DrawText(TextBuffer,120,210,D3DCOLOR_XRGB(0xEA, 0xFF, 0));

					sprintf(TextBuffer,"BestTry: ");
					Direct3D.DrawText(TextBuffer,42,230,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"%d",GameManager.Best);
					Direct3D.DrawText(TextBuffer,120,230,D3DCOLOR_XRGB(0xEA, 0xFF, 0));

					sprintf(TextBuffer,"Points: ");
					Direct3D.DrawText(TextBuffer,42,250,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"%d",GameManager.m_Points);
					Direct3D.DrawText(TextBuffer,120,250,D3DCOLOR_XRGB(0xEA, 0xFF, 0));

					sprintf(TextBuffer,"Rating: ");
					Direct3D.DrawText(TextBuffer,42,270,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(TextBuffer,"%2.1f",GameManager.HighScore);
					Direct3D.DrawText(TextBuffer,120,270,D3DCOLOR_XRGB(0xEA, 0xFF, 0));
}