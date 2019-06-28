#include "RessourceManager.h"


// Konstruktor - Initialisierungen
CResourceManager::CResourceManager()
{
    m_lpD3DDevice = NULL;

    m_Frequency = 0; 
    m_LastCount = 0;
    m_CurCount  = 0;  
    m_Framerate = 60;
    m_Frames    = 0;
}

// Destruktor - Freigabe der Texturen aus der Map
CResourceManager::~CResourceManager()
{
	map<string,LPDIRECT3DTEXTURE9>::iterator it;

	for(it = m_MapTextures.begin(); it != m_MapTextures.end();it++)
	{
        (it->second)->Release();
	}
}

// Initialisierung des Ressourcenmanagers
BOOL CResourceManager::Init(LPDIRECT3DDEVICE9 lpD3DDevice)
{
    m_lpD3DDevice = lpD3DDevice;

    // Frequenz speichern, wenn Counter vorhanden
    if(!QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency))
    {
        MessageBox(0,"Performancecounter nicht vorhanden.","Fehler",MB_OK);
        return FALSE;
    }

    // aktuellen Z�hlerstand Zeit speichern
    QueryPerformanceCounter((LARGE_INTEGER*)&m_CurCount);

    m_LastCount = m_CurCount;

    return TRUE;
}

void CResourceManager::Update(void)
{
    // aktuellen Z�hlerstand holen
    QueryPerformanceCounter((LARGE_INTEGER*)&m_CurCount);

    // testen, ob eine Sekunde vor�ber ist
    if(m_CurCount - m_LastCount > m_Frequency)
    {
        // Wenn ja, enth�lt Frames die
        // aktuelle Framerate
        m_LastCount = m_CurCount;
        m_Framerate = m_Frames;
        m_Frames = 0;

        if(0 == m_Framerate)
        {
            m_Framerate = 1;
        }
    }
    // ansonsten Frames erh�hen
    else
    {
        m_Frames++;
    }
}

int CResourceManager::GetFramerate(void)
{
    if(0 == m_Framerate)
    {
        m_Framerate = 1;
    }
    
    return m_Framerate;
}

// Textur suchen und ggf. zur�ckgeben
LPDIRECT3DTEXTURE9 CResourceManager::GetTexture(LPCSTR lpTextureName)
{
    // Iterator f�r die Suche in der Map
    map<string,LPDIRECT3DTEXTURE9>::iterator result;

    // Nach dem Namen der Textur suchen
    result = m_MapTextures.find(lpTextureName);

    // wenn der Name nicht in der Map vorhanden ist
    // NULL zur�ckgeben
    if(result == m_MapTextures.end())
    {
        return NULL;
    }

    // sonst den Zeiger auf die Textur zur�ckgeben
    return result->second;
}

// Textur hinzuf�gen
void CResourceManager::AddTexture(LPCSTR lpFileName)
{
    // testen, ob die Textur bereits vorhanden ist
    LPDIRECT3DTEXTURE9 lpTexture = GetTexture(lpFileName);

    // wenn sie nicht vorhanden ist, wird sie wie
    // gewohnt erzeugt
    if(!lpTexture)
    {
        D3DXIMAGE_INFO ImageInfo;
        D3DXGetImageInfoFromFile(lpFileName,&ImageInfo);

        D3DXCreateTextureFromFileEx(m_lpD3DDevice,
                                    lpFileName,
                                    ImageInfo.Width,
                                    ImageInfo.Height,
                                    1,0,
                                    D3DFMT_UNKNOWN,
                                    D3DPOOL_MANAGED,
                                    D3DX_FILTER_NONE,
                                    D3DX_FILTER_NONE,
                                    0,0,0,
                                    &lpTexture);

        // und in die Map unter ihrem Namen eingef�gt
        m_MapTextures[lpFileName] = lpTexture;
    }
}