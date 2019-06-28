/********************************************************/
/*	Project:		frosty.Studios 2d Game SDK			*/
/*	Author:			jérôme foucauld						*/
/*	Date:			08.Feb.2005							*/
/*	Platform:		win32 / DirectX						*/
/*	last edited:	08.Feb.2005							*/
/********************************************************/

#include <d3d9.h>
#include <d3dx9.h>
#include "fsUtility.h"


class CDirectXGraphics
{
public:
		CDirectXGraphics();
		virtual ~CDirectXGraphics();

		bool Init(HWND hWnd, short width, short height, bool windowmode	);
		
        // Farbe für den Backbuffer festlegen
        void SetClearColor(D3DCOLOR Color);

        // Text an Position x,y ausgeben
        void DrawText(LPCSTR Text, 
                  int x, int y,
                  D3DCOLOR TextColor);

        // Methoden zum Start/Beenden der Szene 
        void BeginScene(void);
        void EndScene(void);

		// das Direct3D-Device zurückgeben
        LPDIRECT3DDEVICE9 GetDevice(void);


protected:


        // Erzeugen der D3D-Schrift
        void CreateFont(void);
        // Freigeben der Objekte
        void Shutdown(void);

        // Farbe für den Backbuffer
        D3DCOLOR			m_ClearColor;

        // Direct3D Variablen
        LPDIRECT3D9			m_lpD3D;
        LPDIRECT3DDEVICE9	m_lpD3DDevice;
        LPD3DXFONT			m_lpD3DFont;
		LPDIRECT3DSURFACE9	m_lpD3DSurface;
		LPDIRECT3DSURFACE9	m_lpBackBuffer;
};