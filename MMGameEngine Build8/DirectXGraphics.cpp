#include "DirectXGraphics.h"
CFSLogger gLog;


CDirectXGraphics::CDirectXGraphics()
{
	gLog.out("##Initializing DirectXGraphics");
	m_lpD3D				= NULL;
    m_lpD3DDevice		= NULL;
    m_lpD3DFont			= NULL;
	m_lpD3DSurface		= NULL;
	m_lpBackBuffer	= NULL;
}

CDirectXGraphics::~CDirectXGraphics()
{
	gLog.out("##Shutting Down DirectXGraphics");
	Shutdown();
}


bool CDirectXGraphics::Init(HWND hWnd, short width, short height, bool windowmode)
{
	m_lpD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_lpD3D == NULL)
	{
		gLog.out("ERROR could not create Direct3D Object");
		return false;
	}

	// Parameter für den Mode festlegen
	D3DPRESENT_PARAMETERS PParams;
	ZeroMemory(&PParams, sizeof(PParams));

	PParams.SwapEffect			= D3DSWAPEFFECT_DISCARD;
	PParams.hDeviceWindow		= hWnd;
	PParams.Windowed			= windowmode;
	PParams.PresentationInterval= D3DPRESENT_INTERVAL_IMMEDIATE;		// VSync Deaktivieren
	PParams.BackBufferWidth		= width;
	PParams.BackBufferHeight	= height;
	PParams.BackBufferFormat	= D3DFMT_A8R8G8B8;

	// Direct3D Device anlegen
	if (FAILED(m_lpD3D->CreateDevice(
									D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&PParams,
									&m_lpD3DDevice)))
	{
		gLog.out("ERROR Konnte Direct3D Device nicht anlegen!");
		return false;
	}
	// Offscreen Surface
	m_lpD3DDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,&m_lpD3DSurface,0);
	m_lpD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&m_lpBackBuffer);

	CreateFont();
	return true;

}

void CDirectXGraphics::Shutdown()
{
	if(m_lpD3DFont != NULL)
    {
        m_lpD3DFont->Release();
        m_lpD3DFont = NULL;
    }

	if(m_lpD3DDevice != NULL)
	{
		m_lpD3DDevice->Release();
		m_lpD3DDevice = NULL;
	}
	
	if(m_lpD3D != NULL)
	{
		m_lpD3D->Release();
		m_lpD3D = NULL;
	}
}

// Farbe für den Backbuffer festlegen
void CDirectXGraphics::SetClearColor(D3DCOLOR Color)
{
    m_ClearColor = Color;
}

// Text an Position x,y ausgeben
void CDirectXGraphics::DrawText(LPCSTR Text, int x, int y, D3DCOLOR TextColor)
{
    RECT r = { x, y, 0, 0 };

    // Größe des Rechtecks berechnen
    m_lpD3DFont->DrawText(NULL, Text, -1, &r, DT_CALCRECT, TextColor);
	
	// Text ausgeben
    m_lpD3DFont->DrawText(NULL, Text, -1, &r, DT_CENTER, TextColor); 
	
}

void CDirectXGraphics::BeginScene(void)
{
    // Puffer mit schwarz füllen
    m_lpD3DDevice->Clear(0,0,D3DCLEAR_TARGET, m_ClearColor, 0,0);

    m_lpD3DDevice->BeginScene();
}

void CDirectXGraphics::EndScene(void)
{
    m_lpD3DDevice->EndScene();

    m_lpD3DDevice->Present(0,0,0,0);
}
 
// Erzeugen der D3D-Schrift
void CDirectXGraphics::CreateFont(void)
{
    // Struktur für die Beschreibung der Schriftart
    // anlegen und Elemente mit 0 initialisieren
    D3DXFONT_DESC FontDesc = { 0 };

    strcpy(FontDesc.FaceName, "Verdana");
	FontDesc.Height = 14;

    // nach erfolgreichem Aufruf zeigt lpD3DFont 
    // auf ein Objekt vom Typ D3DXFont
    D3DXCreateFontIndirect(m_lpD3DDevice,
                           &FontDesc,
                           &m_lpD3DFont);
}


LPDIRECT3DDEVICE9 CDirectXGraphics::GetDevice(void)
{
    return m_lpD3DDevice;
}