#ifndef _CResourceManager_
#define _CResourceManager_

#include <d3d9.h>
#include <d3dx9.h>

#pragma warning(disable : 4786)  // Warnung 4786 ausschalten

#include <map>
#include <string>


using namespace std;


class CResourceManager
{
    public:
        CResourceManager();
        virtual ~CResourceManager();
        
        // Initialisierung des Ressourcenmanagers
        BOOL Init(LPDIRECT3DDEVICE9 lpD3DDevice);
        
        // mit dieser Methode werden Texturen hinzugefügt
        void AddTexture(LPCSTR lpFileName);
        
        // diese Methode gibt einen Zeiger auf die Textur lpTextureName zurück
        LPDIRECT3DTEXTURE9 GetTexture(LPCSTR lpTextureName);

        // Framerate zurückgeben
        int GetFramerate(void);

        void Update(void);

    //protected:

        // Zeiger auf das Direct3D-Device
        LPDIRECT3DDEVICE9 m_lpD3DDevice;
        
        // diese Map speichert den Namen und den Zeiger auf die Textur
        map<string,LPDIRECT3DTEXTURE9> m_MapTextures;

        // Frequenz des Counters
        LONGLONG m_Frequency; 

        // Zählerstand zu Beginn der letzten Sekunde
        LONGLONG m_LastCount;

        // aktueller Zählerstand 
        LONGLONG m_CurCount;  

        // die Framerate 
        unsigned int m_Framerate;

        // aktueller Frame
        int m_Frames;

};

#endif