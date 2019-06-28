
#include "CollisionArea.h"
#include <stdio.h>

// Attribute der Klasse initialisieren
CCollisionArea::CCollisionArea() : m_lpArea(0), 
								   m_nAreaWidth(0), 
								   m_nAreaHeight(0),
								   m_nReductionFactor(0)
{

}

CCollisionArea::~CCollisionArea()
{
	// Feld löschen
	delete[] m_lpArea;
}

// Feld für die Kollisionserkennung erzeugen.
void CCollisionArea::Create(DWORD nWidth,DWORD nHeight)
{
	m_nAreaWidth  = nWidth;
	m_nAreaHeight = nHeight;
	m_lpArea  = new BYTE[m_nAreaWidth*m_nAreaHeight];
}


// testen, ob eine Kollision aufgetreten ist
BOOL CCollisionArea::TestCollision(CSprite* Sprite1,CSprite* Sprite2)
{
	
	// Auflösungsreduziertes Image der Sprites
	BYTE* RedImg1 = Sprite1->GetReducedImage();
	BYTE* RedImg2 = Sprite2->GetReducedImage();

	// Breite und Höhe
	DWORD Width1  = Sprite1->m_Width;
	DWORD Height1 = Sprite1->m_Height;

	DWORD Width2  = Sprite2->m_Width;
	DWORD Height2 = Sprite2->m_Height;

	// sowie die Positionen der Sprite holen
	D3DXVECTOR3 vPos1 = Sprite1->GetPosition();
	D3DXVECTOR3 vPos2 = Sprite2->GetPosition();

	// Feld löschen (geht auch effizienter indem man nur den
	// Bereich, den das erste Sprite einnimmt löscht)
	memset(m_lpArea,0,m_nAreaWidth*m_nAreaHeight);

	DWORD i,j;
	DWORD x,y;

	x = (DWORD)vPos1.x;
	y = (DWORD)vPos1.y;

	Width1  /= 1;
	Height1 /= 1;

	Width2  /= 1;
	Height2 /= 1;

	// erstes Sprite in den Puffer zeichnen
	for(i=0;i<Height1;i++)
	{
		for(j=0;j<Width1;j++)
		{
           
			m_lpArea[(i+y)*m_nAreaWidth+j+x] = RedImg1[i*Width1+j];
		}	
	}


	x = (DWORD)vPos2.x;
	y = (DWORD)vPos2.y;

	// testen, ob ein Pixel des zweiten Sprites über
	// einen bereits gesetzen Pixel gezeichnet würde
	for(i=0;i<Height2;i++)
	{
		for(j=0;j<Width2;j++)
		{
			// wenn ja ist eine Kollision aufgetreten
			if(m_lpArea[(i+y)*m_nAreaWidth+j+x] && RedImg2[i*Width2+j])
			{
				return TRUE;
			}
		}	
	}

	return FALSE;
}
