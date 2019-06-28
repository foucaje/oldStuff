#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define USE_KEYBOARD 0
#define USE_MOUSE    1


#define MOVE_LEFT      1
#define MOVE_RIGHT     2
#define MOVE_UP        4
#define MOVE_DOWN      8
#define MOVE_FIRE     16

class CInputManager
{
public:

    // Konstruktor und Destruktor
    CInputManager(void);
    virtual ~CInputManager();

    // Eingaben vom einem Gerät zurückgeben
    int GetInput(void);
    
    // den InputManager initialisieren, Standardgerät ist die Tastatur
    BOOL Init(HWND hWnd,HINSTANCE hInst,int Device = USE_KEYBOARD);

private:

    // Zeiger auf das DirectInput8-Objekt
    LPDIRECTINPUT8 m_lpDI; 

    // Zeiger auf das Gerät : Maus, Joystick oder Tastatur
    LPDIRECTINPUTDEVICE8 m_lpDIDevice;

    // das gewählte Gerät
    int m_CurrentDevice;

    // die verschiedenen Geräte initialisieren
    BOOL initKeyboard(HWND hWnd);
    BOOL initMouse(HWND hWnd);
    
	// Eingaben der Geräte erhalten
    int getKeyboardInput(void);
    int getMouseInput(void);
};