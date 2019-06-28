#ifndef _CollisionArea_h
#define _CollisionArea_h

#include "sprite.h"


// diese Klasse testet die Kollision von zwei Sprites
// die Sprites müssen da
class CCollisionArea  
{
	public:
		CCollisionArea();
		virtual ~CCollisionArea();

		// das Feld erzeugen
		void Create(DWORD Width,DWORD Height);

		// Methode gibt TRUE zurück, wenn eine Kollision 		
		// aufgetreten ist, FALSE sonst
		BOOL TestCollision(CSprite* Sprite1,CSprite* Sprite2);

	protected:

		// das Feld für den Kollisionstest
		BYTE* m_lpArea;
		
		// Breite und Höhe des Feldes
		DWORD m_nAreaWidth;
		DWORD m_nAreaHeight;

		DWORD m_nReductionFactor;
};

#endif