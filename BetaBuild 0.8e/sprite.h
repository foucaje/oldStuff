#ifndef _sprite_h
#define _sprite_h

#include <d3d9.h>
#include <d3dx9.h>
#include "fsUtility.h"


// Definition der Bewegungsrichtungen
//enum DIRECTION { LEFT, RIGHT, UP, DOWN };

// Definition der Spriteklasse
class CSprite
{
    public:
       CSprite();
       virtual ~CSprite();

       // Erzeugung des Sprites
       void Create(LPDIRECT3DDEVICE9 lpDevice,
                   LPCSTR lpFileName);
        
       // Sprite zeichnen
       void Draw(void);
        
       // absolute Position festlegen
       void SetPosition(float x,float y);
	   
	   // Position zurückgebebn
	   void GetPosition(float &x, float &y);
	   D3DXVECTOR3 GetPosition();

       // absolute Rotation festlegen
       void SetRotation(float rotation);
        
       // absolute Skalierung festlegen
       void SetScale(float scale);
        
       // Schrittweite festlegen
       void SetStep(float StepX,float StepY);

       // relative Bewegung um StepX,StepY Pixel
       short Move(int dir, bool typ);
        
       // relative Skalierung 
       void Scale(float dScale);
        
       // relative Rotation
       void Rotate(float dRotation);

	   // Breite und Höhe
       int   m_Width;
       int   m_Height;
	   
	   // Breite und höhe des Sprites zurückgeben
	   DWORD GetWidth(void);
	   DWORD GetHeight(void);

       // Reduziertes Sprite (Image) zurückgeben
	   BYTE* GetReducedImage(void);
	   
	   void AddTexture(LPDIRECT3DTEXTURE9 lpTexture);

     protected:

		// Zeiger auf die reduzierte Textur
       BYTE* m_lpReducedImage;

       // Zeiger auf das Direct3D Device
       LPDIRECT3DDEVICE9 m_lpDevice;
        
       // Zeiger auf das ID3DXSprite-Objekt
       LPD3DXSPRITE m_lpSprite;
    
       // Zeiger auf die Textur
       LPDIRECT3DTEXTURE9 m_lpTexture;

       // aktuelle Position
       D3DXVECTOR3 m_vPosition;
   
       // Skalierung
       D3DXVECTOR3 m_vScale;
    
       // Mittelpunkt
       D3DXVECTOR3 m_vCenter;

	  
	   // Rotationsmittelpunkt
       //D3DXVECTOR3 m_vRotationCenter;


       // aktuelle Rotation
       float m_Rotation;
	   short m_Direction;
	   short m_Look;
   
       // Schrittweite
       float m_StepX;
       float m_StepY;

       
};

#endif