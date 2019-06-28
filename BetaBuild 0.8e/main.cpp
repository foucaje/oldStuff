/********************************************************/
/*	Project:		nexusCreations 2d Game SDK			*/
/*	Author:			jérôme foucauld						*/
/*	Date:			08.Feb.2005							*/
/*	Platform:		win32 / DirectX						*/
/*	last edited:	17.Feb.2005							*/
/********************************************************/

// Headerdateien
#include "DirectXGraphics.h"
#include "sprite.h"
#include "RessourceManager.h"
#include "InputManager.h"
#include "DXAudioClass.h"
#include "GameManager.h"
#include <ctime>
#include "CollisionArea.h"

short FrameRate = 15;

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
CGameManager		GameManager2;			// todo: verbessern
CSprite				Snake;					// ok
CSprite				Snake2;					// ok
CSprite				BG;						// ok
CSprite				Food;					// ok
CCollisionArea		CollisionArea;			// ok

char StatusBuffer[256];
char TextBuffer[256];
BOOL InitGame(void);
void CreateSnake(void);
void CreateFood(void);
void GetFood(void);

LONGLONG Frequency;
LONGLONG Offset;
LONGLONG NextFrame;
LONGLONG CurCount;
float FrameMod = 1.0f;
int SnakeLen = 1;
int Snake2Len = 1;


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
		
	// Kollisionsfeld erzeugen
	CollisionArea.Create(Game.m_scrWidth,Game.m_scrHeight);
	CreateFood();

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

				if (CurCount > NextFrame)
				{
				ResourceManager.Update();

				if (Game.GetGameState() == 0)
				{
					Direct3D.BeginScene();
					Direct3D.ShowIntro();
					int Key = InputManager.GetInput();
					sprintf(TextBuffer,"Letzte Punktzahl: %d", GameManager.GetPoints());
					Direct3D.DrawText(TextBuffer,Game.m_scrWidth/2-65,Game.m_scrHeight/2-80,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));

					if (Key==16)
						{	
						Game.m_GameState = 1;
						Snake.SetPosition((float)Game.m_scrWidth / 2, (float)Game.m_scrHeight / 2);
						GameManager.Reset();
						SnakeLen=1;
						FrameMod = 1.0f;
						}
					if (Key==32)
						{	
						Game.m_GameState = 2;
						Snake.SetPosition((float)300.0f, (float)300.0f);
						Snake2.SetPosition((float)700.0f, (float)300.0f);
						GameManager.Reset();
						GameManager2.Reset();
						SnakeLen=1;
						Snake2Len=1;
						}
					Direct3D.EndScene();
				}
			
				if (Game.GetGameState() == 1)
				{
					Direct3D.BeginScene();
					BG.Draw();
					int Key = InputManager.GetInput();
					if (Key == 70)
						FrameMod+=0.1f;
					if (Key == 71)
						FrameMod-=0.1f;

					if (Snake.Move(Key,false)==0)
					{
						if (GameManager.SetDestroyed()==0)
						{
						DXAudio.PlaySoundEffect("gameover.wav");
						Game.m_GameState=0;
						}
					}
					GetFood();
					Food.Draw();
					
					if (GameManager.Update()==0)
					{
					DXAudio.PlaySoundEffect("gameover.wav");
					Game.m_GameState=0;
					}
					
					switch(SnakeLen)
					{
					case 5:
						FrameMod = 0.9f;
						break;
					case 10:
						FrameMod = 0.8f;
						break;
					case 15:
						FrameMod = 0.7f;
						break;
					case 20:
						FrameMod = 0.6f;
						break;
					case 25:
						FrameMod = 0.5f;
						break;
					case 30:
						FrameMod = 0.4f;
						break;
					case 40:
						FrameMod = 0.3f;
						break;
					case 50:
						FrameMod = 0.2f;
						break;
					}

					GameManager.Draw();
					Snake.Draw();
					sprintf(StatusBuffer,"Speed: %.1f", FrameMod);
					Direct3D.DrawText(StatusBuffer,170,740,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					sprintf(StatusBuffer,"Framerate: %3d",ResourceManager.GetFramerate());
					Direct3D.DrawText(StatusBuffer,170,720,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					// PlayerScore	
					sprintf(TextBuffer,"%d", GameManager.GetPoints());
					Direct3D.DrawText(TextBuffer,200,672,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					Direct3D.EndScene();
				}

				if (Game.GetGameState() == 2)
				{
					Direct3D.BeginScene();
					BG.Draw();
					int Key = InputManager.GetInput();
					if (Snake.Move(Key, false)==0)
					{
						if (GameManager.SetDestroyed()==0)
						{
						DXAudio.PlaySoundEffect("gameover.wav");
						Game.m_GameState=0;
						}
						Key=0;
					}
					
					Key = InputManager.GetInput();
					if (Snake2.Move(Key, true)==0)
					{
						if (GameManager2.SetDestroyed()==0)
						{
						DXAudio.PlaySoundEffect("gameover.wav");
						Game.m_GameState=0;
						}
					}
					GetFood();
					Food.Draw();
					
					if (GameManager.Update()==0)
					{
						DXAudio.PlaySoundEffect("gameover.wav");
						Game.m_GameState=0;
					}
					if (GameManager2.Update()==0)
					{
						DXAudio.PlaySoundEffect("gameover.wav");
						Game.m_GameState=0;
					}

					if (CollisionArea.TestCollision(&Snake2,&Snake))
						Game.m_GameState=0;

					GameManager.Draw();
					Snake.Draw();
					GameManager2.Draw();
					Snake2.Draw();
					

					// Framerate zu Debug zwecken
					//sprintf(StatusBuffer,"Framerate: %3d",ResourceManager.GetFramerate());
					//Direct3D.DrawText(StatusBuffer,180,700,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					/*
					// Leben
					sprintf(TextBuffer,"%d", GameManager.GetLives());
					Direct3D.DrawText(TextBuffer,180,672,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));*/
			
					// PlayerScore	
					sprintf(TextBuffer,"%d", GameManager2.GetPoints());
					Direct3D.DrawText(TextBuffer,590,672,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));	

					sprintf(TextBuffer,"%d", GameManager.GetPoints());
					Direct3D.DrawText(TextBuffer,200,672,D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
					Direct3D.EndScene();
				}
				NextFrame=CurCount + (Offset*FrameMod);
				}
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
                          "BattleSnake (c)'2005 nexusCreations",	// Fenstertitel
                          WS_OVERLAPPEDWINDOW |						// Fenster
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
                 // Wenn ESC gedrückt, Anwendung beenden
                 case VK_ESCAPE:
                         DestroyWindow(hWnd);
                 break;
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

	if(!Game.m_windowmode)
    {
        ShowCursor(FALSE);
    }

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
    DXAudio.AddBackgroundMusic("musik.mid");
    DXAudio.PlayBackgroundMusic();

    // Soundeffekte laden
    DXAudio.AddSound("aua.wav");
	DXAudio.AddSound("rulps.wav");
	DXAudio.AddSound("mjam.wav");
	DXAudio.AddSound("gulp.wav");
	DXAudio.AddSound("mmh.wav");

	DXAudio.AddSound("hallo.wav");
	DXAudio.AddSound("gameover.wav");
    
    // GameManager initialisieren
	GameManager.Init(Direct3D.GetDevice(), &ResourceManager, &DXAudio, &Snake);
	GameManager2.Init(Direct3D.GetDevice(), &ResourceManager, &DXAudio, &Snake2);

	ResourceManager.AddTexture("res\\SnakeBody.png");
	ResourceManager.AddTexture("res\\SnakeBodyB.png");
	
    CreateSnake();
	BG.Create(Direct3D.GetDevice(),"res\\playbg.png");
	mLog.out("InitGame() successfull");
    return TRUE;
}

// die Schlange erstellen
void CreateSnake(void)
{
    // Sprite für die Schlange anlegen
    Snake.Create(Direct3D.GetDevice(),"res\\SnakeKopf.png");
	Snake.SetStep(10.0f, 10.0f);
	Snake2.Create(Direct3D.GetDevice(),"res\\SnakeKopfB.png");
	Snake2.SetStep(10.0f, 10.0f);
	DXAudio.PlaySoundEffect("hallo.wav");
}

// neues FressiFressi generieren
void CreateFood(void)
{
    Food.Create(Direct3D.GetDevice(),"res\\Food.png");
	srand (timeGetTime());
	Food.SetPosition(rand()%660+130,rand()%530+40);
}

// auf Collision mit Essen prüfen
void GetFood(void)
{
	if(CollisionArea.TestCollision(&Snake,&Food))
	{
		srand(timeGetTime());
		
		switch(rand()%4+1)
		{
		case 1:
			DXAudio.PlaySoundEffect("gulp.wav");
			break;
		case 2:
			DXAudio.PlaySoundEffect("rulps.wav");
			break;
		case 3:
			DXAudio.PlaySoundEffect("mmh.wav");
			break;
		case 4:
			DXAudio.PlaySoundEffect("mjam.wav");
			break;
		}
				
		Food.SetPosition(rand()%660+130,rand()%530+40);
		GameManager.m_Points+=1*SnakeLen;
		GameManager.CreateBody(Snake.GetPosition(),0);
		SnakeLen++;
	}

	if(CollisionArea.TestCollision(&Snake2,&Food))
	{
		srand(timeGetTime());
		
		switch(rand()%4+1)
		{
		case 1:
			DXAudio.PlaySoundEffect("gulp.wav");
			break;
		case 2:
			DXAudio.PlaySoundEffect("rulps.wav");
			break;
		case 3:
			DXAudio.PlaySoundEffect("mmh.wav");
			break;
		case 4:
			DXAudio.PlaySoundEffect("mjam.wav");
			break;
		}
				
		Food.SetPosition(rand()%660+130,rand()%530+40);
		GameManager2.m_Points+=1*Snake2Len;
		GameManager2.CreateBodyB(Snake2.GetPosition(),0);
		Snake2Len++;
	}
}
