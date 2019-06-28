#include "InputManager.h"
#include <stdio.h>


CInputManager::CInputManager(void)
{
    m_CurrentDevice = 0;
    m_lpDI = NULL;
    m_lpDIDevice = NULL;
}

CInputManager::~CInputManager()
{
    // Eingabegerät freigeben
    if(NULL != m_lpDIDevice)
    {
        m_lpDIDevice->Unacquire();
        m_lpDIDevice->Release();
        m_lpDIDevice = NULL;
    }
    
    // DirectInput8-Objekt freigeben
    if(NULL != m_lpDI)
    {
        m_lpDI->Release();
        m_lpDI = NULL;
    }
}


BOOL CInputManager::Init(HWND hWnd,HINSTANCE hInst,int Device)
{
    m_CurrentDevice = Device;
    
    // DirectInput8-Objekt anlegen
    if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, 
                                 IID_IDirectInput8,(void**)&m_lpDI,NULL)))
    { 
        MessageBox(hWnd,"Fehler beim Anlegen des DirectInput8-Objekts","Fehler",MB_OK);
        return FALSE;
    } 

    // gewähltes Gerät initialisieren
    switch(m_CurrentDevice)
    {
        case USE_KEYBOARD:
                return initKeyboard(hWnd);
            break;
        case USE_MOUSE:
                return initMouse(hWnd);
            break;
        default:
                m_CurrentDevice = USE_KEYBOARD;
                return initKeyboard(hWnd);
    }
    return TRUE;
}

int CInputManager::GetInput(void)
{
    // Eingabe des gewählten Geräts zurückgeben
    if(USE_MOUSE == m_CurrentDevice)
    {
        return getMouseInput();
    }    
    else 
    {
        return getKeyboardInput();
    }

}


int CInputManager::getKeyboardInput(void)
{
       // alle Tasten der Tastatur
        char     buffer[256]; 
    
        if(FAILED(m_lpDIDevice->GetDeviceState(sizeof(buffer),(LPVOID)&buffer))) 
        { 
            // die Tastatur wurde zwischenzeitlich 
            // durch eine andere Anwendung genutzt
            // Kontrolle wiederholen
            m_lpDIDevice->Acquire();
            return 0;
        } 
    
        // bestimmen welche Taste gedrückt wurde
        // und Tastenkombination zurückgeben
        int Keys = 0;

        if(buffer[DIK_LEFT])
        {
            Keys = MOVE_LEFT;       
        }
        if(buffer[DIK_RIGHT])
        {
            Keys = MOVE_RIGHT;
        }
        if(buffer[DIK_UP])
        {
            Keys = MOVE_UP;
        }
        if(buffer[DIK_DOWN])
        {
            Keys = MOVE_DOWN;           
        }
        
        if(buffer[DIK_F1])
        {
            Keys = 16;
        }else if(buffer[DIK_F2])
        {
            Keys = 32;
        }
		
		if(buffer[DIK_F5])
		{
			Keys = 64;
		}else if(buffer[DIK_F6])
		{
			Keys = 128;
		}


		if(buffer[DIK_SPACE])
			Keys = 99;


		if(buffer[DIK_ADD])
        {
            Keys = 70;           
        }

		if(buffer[DIK_SUBTRACT])
        {
            Keys = 71;           
        }

		if(buffer[DIK_ESCAPE])
        {
            Keys = 98;           
        }
		if(buffer[DIK_Q]) Keys = 200;
        if(buffer[DIK_W]) Keys = 201;
        if(buffer[DIK_E]) Keys = 202;
		if(buffer[DIK_R]) Keys = 203;
    	if(buffer[DIK_T]) Keys = 204;
		if(buffer[DIK_Z]) Keys = 205;
		if(buffer[DIK_U]) Keys = 206;
        if(buffer[DIK_I]) Keys = 207;
        if(buffer[DIK_O]) Keys = 208;
        if(buffer[DIK_P]) Keys = 209;
		
		if(buffer[DIK_A]) Keys = 210;
        if(buffer[DIK_S]) Keys = 211;
        if(buffer[DIK_D]) Keys = 212;
		if(buffer[DIK_F]) Keys = 213;
    	if(buffer[DIK_G]) Keys = 214;
		if(buffer[DIK_H]) Keys = 215;
		if(buffer[DIK_J]) Keys = 216;
        if(buffer[DIK_K]) Keys = 217;
        if(buffer[DIK_L]) Keys = 218;
        
		if(buffer[DIK_Y]) Keys = 219;
        if(buffer[DIK_X]) Keys = 220;
        if(buffer[DIK_C]) Keys = 221;
		if(buffer[DIK_V]) Keys = 222;
    	if(buffer[DIK_B]) Keys = 223;
		if(buffer[DIK_N]) Keys = 224;
		if(buffer[DIK_M]) Keys = 225;
        if(buffer[DIK_RETURN]) Keys = 230;
		if(buffer[DIK_BACK]) Keys = 231;
        
		
		return Keys;
}


int CInputManager::getMouseInput(void)
{
    DWORD NumElements = 1;
    DIDEVICEOBJECTDATA data;
    ZeroMemory(&data,sizeof(data));

    if(FAILED(m_lpDIDevice->GetDeviceData(sizeof(data),&data,&NumElements,0))) 
    { 
        // die Mouse wurde zwischenzeitlich 
        // von einer andere Anwendung genutzt
        // Kontrolle wiederholen
        m_lpDIDevice->Acquire();
        return 0;
    } 

    int MouseInput = 0;

    int Movement = data.dwData;

    // bestimmen, in welche Richtung die Mouse bewegt wurde 
    // und ob eine Mouse-Taste gedrückt wurde
    switch(data.dwOfs)
    {
        case DIMOFS_X:            
                if(Movement < 0)
                {
                    MouseInput += MOVE_LEFT;
                }
                else
                {
                    MouseInput += MOVE_RIGHT;
                }                
            break;

        case DIMOFS_Y:
                if(Movement < 0)
                {
                    MouseInput += MOVE_UP;
                }
                else
                {
                    MouseInput += MOVE_DOWN;
                }                            
            break;

        case DIMOFS_BUTTON0:
                MouseInput += MOVE_FIRE;
            break;
    }
    
    return MouseInput;
}
    
BOOL CInputManager::initKeyboard(HWND hWnd)
{
    // Gerät festlegen
    if(FAILED(m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpDIDevice, NULL)))
    {
        MessageBox(hWnd,"Fehler beim Anlegen des Geräts","Fehler",MB_OK);
        return FALSE;
    }

    // c_dfDIKeyboard ist eine vordefinierte Variable
    if(FAILED(m_lpDIDevice->SetDataFormat(&c_dfDIKeyboard)))
    {
        MessageBox(hWnd,"Fehler beim Setzen des Datenformats","Fehler",MB_OK);        
        return FALSE;        
    }
 
    // Kooperationsebene festlegen
    if(FAILED(m_lpDIDevice->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | 
                                                     DISCL_NONEXCLUSIVE)))
    {    
        MessageBox(hWnd,"Fehler beim Setzen des Kooperationsebene (DirectInput)","Fehler",MB_OK);
        return FALSE;
    }
	
    // Zugang zur Tastatur verschaffen
    m_lpDIDevice->Acquire();

    return TRUE;
}


BOOL CInputManager::initMouse(HWND hWnd)
{
 // Gerät für die Mouse festlegen
    if(FAILED(m_lpDI->CreateDevice(GUID_SysMouse, &m_lpDIDevice, NULL)))
    {
        MessageBox(hWnd,"Fehler beim Anlegen des Geräts","Fehler",MB_OK);
        return FALSE;
    }

    // c_dfDIMouse ist eine vordefinierte Variable
    if(FAILED(m_lpDIDevice->SetDataFormat(&c_dfDIMouse)))
    {
        MessageBox(hWnd,"Fehler beim Setzen des Datenformats","Fehler",MB_OK);        
        return FALSE;        
    }
 
    // Kooperationsebene festlegen
    if(FAILED(m_lpDIDevice->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | 
                                                  DISCL_NONEXCLUSIVE)))
    {    
        MessageBox(hWnd,"Fehler beim Setzen des Kooperationsebene (DirectInput)","Fehler",MB_OK);
        return FALSE;
    }
 
    // Eigenschaften für das Gerät setzen (Puffergröße)
    DIPROPDWORD DIProperties;
    ZeroMemory(&DIProperties,sizeof(DIProperties));
    DIProperties.diph.dwSize       = sizeof(DIPROPDWORD);
    DIProperties.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    DIProperties.diph.dwObj        = 0;
    DIProperties.diph.dwHow        = DIPH_DEVICE;
    // Puffer größe festlegen
    DIProperties.dwData            = 16;
 
    // Puffergröße für Mouseeingaben setzen
    m_lpDIDevice->SetProperty(DIPROP_BUFFERSIZE, &DIProperties.diph);

    // Zugang zur Mouse verschaffen
    m_lpDIDevice->Acquire();
    return TRUE;
}
